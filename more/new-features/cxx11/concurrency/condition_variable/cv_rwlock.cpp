#include <iostream>
#include <future>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>
#include <cassert>
#include <string>
#include <condition_variable>
#include <shared_mutex>
#include <tuple>
#include <functional>
#include <iomanip>
#include <cstring>
#include <atomic>
#include <tuple>

namespace
{

class nocopyable
{
	public:
		nocopyable() {}
		nocopyable(const nocopyable&) = delete;
		nocopyable& operator=(const nocopyable&) = delete;
};

class ticketlock
{ //FIFO
	public:
		ticketlock() : turn_(0), ticket_(0) {}

		void lock()
		{
			int me = ticket_.fetch_add(1, std::memory_order_relaxed);
			while(me != turn_.load(std::memory_order_acquire))
			{
			}
		}

		void unlock()
		{
			int new_turn = turn_.load(std::memory_order_relaxed); 
			turn_.store(new_turn + 1, std::memory_order_release);
		}

	private:
		std::atomic_int turn_;
		std::atomic_int ticket_;
};

template<typename M=std::mutex>
class rwlock: nocopyable
{ // 存在写饥饿
	public:
		rwlock(): readers_(0){}

		void lock()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			read_queue_.wait(lock, [this]{ return readers_ == 0; });
			readers_ = -1;
		}

		void unlock()
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);
				readers_ = 0;
			}
			read_queue_.notify_all();
		}

		void lock_shared()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			read_queue_.wait(lock, [this]{ return readers_ >= 0; });
			++readers_;
		}

		void unlock_shared()
		{
			int local_count;

			{
				std::lock_guard<std::mutex> lock(mutex_);
				local_count = --readers_;
			}

			if(local_count == 0)// reduce sys_calls
			{
				read_queue_.notify_one();
			}
		}

	private:
		int readers_; 
		std::mutex mutex_;
		std::condition_variable read_queue_;
};

class pthread_rwlock: nocopyable
{ 
	public:
		pthread_rwlock()
		{
			pthread_rwlock_init(&mutex_, nullptr);
		}

		void lock()
		{
			pthread_rwlock_wrlock(&mutex_);
		}

		void unlock()
		{
			pthread_rwlock_unlock(&mutex_);
		}

		void lock_shared()
		{
			pthread_rwlock_rdlock(&mutex_);
		}

		void unlock_shared()
		{
			unlock();
		}

	private:
		pthread_rwlock_t mutex_;
};

class rwlock_write_first: nocopyable
{
	public:
		rwlock_write_first(): readers_(0), writer_entered_(false){}

		void lock()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			write_queue_.wait(lock, [this]{ return !writer_entered_; });
			writer_entered_ = true;
			read_queue_.wait(lock, [this]{ return readers_ == 0; });
		}

		void unlock()
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);
				writer_entered_ = false;
			}
			write_queue_.notify_all();
		}

		void lock_shared()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			write_queue_.wait(lock, [this]{ return !writer_entered_; });
			++readers_;
		}

		void unlock_shared()
		{
			int local_count;
			int local_writer_entered;

			{
				std::lock_guard<std::mutex> lock(mutex_);
				local_count = --readers_;
				local_writer_entered = writer_entered_;
			}

			if(local_writer_entered && local_count == 0)
			{
				read_queue_.notify_one();
			}
		}

	private:
		int readers_; 
		bool writer_entered_;
		std::mutex mutex_;
		std::condition_variable read_queue_;
		std::condition_variable write_queue_;
};

class atomic_rwlock:nocopyable
{
	public:
		atomic_rwlock(): count_(0){}

		void lock()
		{
			int expected = 0;
			while(!count_.compare_exchange_weak(expected,
						-1,
						std::memory_order_acquire,
						std::memory_order_relaxed))
			{
				expected = 0;
			}
		}

		void unlock()
		{
			count_.store(0, std::memory_order_release);
		}

		void lock_shared()
		{
			int expected = count_.load(std::memory_order_relaxed);
			int new_count = expected + 1;
			while(!count_.compare_exchange_weak(expected,
						new_count,
						std::memory_order_acquire,
						std::memory_order_relaxed))
			{
				new_count = expected + 1;
			}
		}

		void unlock_shared()
		{
			count_.fetch_sub(1, std::memory_order_release);
		}

	private:
		std::atomic<int> count_; // -1 when W lock held, > 0 when R locks held.
};


class C
{
	public:
		C(int i = 0)
		{
			data_ = new std::int8_t[kSize];
			for(int k = 0; k < kSize; ++k)
			{
				data_[k] = i;
			}
		}

		~C()
		{
			delete[] data_;
		}

		C(const C& rhs)
		{
			memcpy(data_, rhs.data_, kSize);
		}

		C& operator=(const C& rhs)
		{
			if(&rhs != this)
			{
				memcpy(data_, rhs.data_, kSize);
			}
			return *this;
		}

		void reset()
		{
			delete[] data_;
			data_ = new std::int8_t[kSize];
		}

		static int size()
		{
			return kSize;
		}

		static void size(int sz)
		{
			assert(sz > 0 && sz <= 1024 * 64);
			kSize = sz;
		}
	
	private:
		static int kSize;
		std::int8_t *data_;
};

int C::kSize = 1024;

C data{0};
std::mutex iomutex;
std::atomic<std::uint64_t> number_reading{0};
constexpr std::uint64_t MAX_NUMBER = 1000 * 10000;
constexpr std::uint64_t PER_NUMBER = 400 * 10000;

template<typename M>
std::chrono::steady_clock::duration work_thread(int id, M& mutex)
{
	static thread_local std::random_device seed{};
	static thread_local std::mt19937 generator(seed());
	static thread_local std::uniform_int_distribution<int> distributor(0, 1024);

	pthread_setname_np(pthread_self(), (id == 0)?"writer":"reader");

	auto start = std::chrono::steady_clock::now();
	if(id == 0)
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			if(number_reading.load(std::memory_order_relaxed)% PER_NUMBER == 0)
			{
				std::unique_lock<M> lock(mutex);
				data = distributor(generator);
			}
		}
	}
	else
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			C tmp;
			{
				std::shared_lock<M> lock(mutex);
				tmp = data;
				(void)tmp;
			}
			number_reading.fetch_add(1, std::memory_order_relaxed);
		}
	}
	auto duration = std::chrono::steady_clock::now() - start;
	std::lock_guard<std::mutex> lock(iomutex);
	//std::cout  << id << ": duration=" << duration.count() << std::endl;
	return duration;
}

template<>
std::chrono::steady_clock::duration work_thread(int id, std::mutex& mutex)
{
	static thread_local std::random_device seed{};
	static thread_local std::mt19937 generator(seed());
	static thread_local std::uniform_int_distribution<int> distributor(0, 1024);

	pthread_setname_np(pthread_self(), (id == 0)?"writer":"reader");

	auto start = std::chrono::steady_clock::now();
	if(id == 0)
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			if(number_reading.load(std::memory_order_relaxed)% PER_NUMBER == 0)
			{
				std::lock_guard<std::mutex> lock(mutex);
				data = distributor(generator);
			}
		}
	}
	else
	{
		while(number_reading.load(std::memory_order_relaxed) < MAX_NUMBER)
		{
			C tmp;
			{
				std::lock_guard<std::mutex> lock(mutex);
				tmp = data;
			}
			(void)tmp;
			number_reading.fetch_add(1, std::memory_order_relaxed);
		}
	}
	auto duration = std::chrono::steady_clock::now() - start;
	std::lock_guard<std::mutex> lock(iomutex);
	//std::cout << id << ": duration=" << duration.count() << std::endl;
	return duration;
}

using funct_t = std::function<std::chrono::steady_clock::duration (int)>;
const int N = 7;
const int cpus = std::thread::hardware_concurrency();
const std::string names[N] = {
	"std::mutex",
	"rwlock<std::mutex>",
	"rwlock_write_first",
	"atomic_rwlock",
	"pthread_rwlock",
	"std::shared_mutex",
	"rwlock<ticketlock>"
};

std::mutex mutex;
rwlock mutex1;
rwlock_write_first mutex2;
atomic_rwlock mutex3;
pthread_rwlock mutex4;
std::shared_mutex mutex5;
rwlock<ticketlock> mutex6;

}

int main(void)
{
	assert(cpus > 1);

	funct_t funcs[N];
	funcs[0] = [](int id) { return work_thread(id, mutex); };
	funcs[1] = [](int id) { return work_thread(id, mutex1); };
	funcs[2] = [](int id) { return work_thread(id, mutex2); };
	funcs[3] = [](int id) { return work_thread(id, mutex3); };
	funcs[4] = [](int id) { return work_thread(id, mutex4); };
	funcs[5] = [](int id) { return work_thread(id, mutex5); };
	funcs[6] = [](int id) { return work_thread(id, mutex6); };

	std::cout << "MAX_NUMBER: " << MAX_NUMBER << std::endl;
	std::cout << "PER_NUMBER: " << PER_NUMBER << std::endl;
	std::cout << "cpus:       " << cpus << std::endl;

	std::vector<int> sizes;
	for(int i = 3; i <= 16; ++i)
	{
		sizes.push_back(1 << i);
	}


	for(int sz: sizes)
	{
		C::size(sz);
		data.reset();
		std::cout << "***size:       " << C::size() << std::endl;

		double durations[N] = {0, 0, 0, 0, 0, 0};
		
		for(int k = 0; k < N; ++k)
		{
			//std::cout << "*****" << names[k] << std::endl; 
			std::vector<std::future<std::chrono::steady_clock::duration>> futs;
			for(int i = 0; i < cpus; ++i)
			{
				futs.emplace_back(std::async(funcs[k], i));
			}

			std::chrono::steady_clock::duration duration{0};
			for(auto& fut: futs)
			{
				duration += fut.get();
			}
			//std::cout << "\tduration:   " << duration.count() << std::endl;
			//std::cout << "\taverage:    " << duration.count() / cpus << std::endl;
			//std::cout << "\taverage:    " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / cpus  << "ms" << std::endl;
			number_reading.store(0, std::memory_order_relaxed);
			durations[k] = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() * 1.0 / cpus;
		}
		std::cout << "   statistics(time cost decreased against std::mutex)\n";
		for(int k = 1; k < N; ++k)
		{
			std::cout << "\t" << std::left << std::setw(24) << names[k] << ": " << (durations[k] - durations[0])/ durations[0] * 100  << "%" << std::endl;
		}
		std::cout << "\n\n";
	}
	return 0;
}

#ifndef _THREADSAFE_STD_QUEUE_
#define _THREADSAFE_STD_QUEUE_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class threadsafe_std_queue
{
	public:
		using value_type = T;

		threadsafe_std_queue() {}
		~threadsafe_std_queue() {}
		
		void push(const T& e) ;
		std::unique_ptr<T> try_pop();

		//void wait_and_pop(T& e);
	
	private:
		mutable std::mutex mutex_;
		std::condition_variable cv_not_empty_;
		std::queue<std::unique_ptr<T>> queue_;
};

template<typename T>
void threadsafe_std_queue<T>::push(const T& e)
{
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push(std::make_unique<T>(e));
	}
	//cv_not_empty_.notify_one();
}

template<typename T>
std::unique_ptr<T> threadsafe_std_queue<T>::try_pop()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if(queue_.empty())
	{
		return nullptr;
	}

	auto ret = std::move(queue_.front());
	queue_.pop();
	return ret;
}

//template<typename T>
//void threadsafe_std_queue<T>::wait_and_pop(T& e)
//{
//	std::unique_lock<std::mutex> lock(mutex_);
//	cv_not_empty_.wait(lock, [this]{ return !queue_.empty(); });
//	e = queue_.front();
//	queue_.pop();
//}

#endif

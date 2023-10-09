#ifndef _THREADSAFE_RING_H_
#define _THREADSAFE_RING_H_
#include <vector>
#include <mutex>

template<typename T>
class threadsafe_ring
{ // threadsafe_ring queue
	public:

		using value_type = T;

		threadsafe_ring(int capacity):
			capacity_(capacity),
			head_(0),
			tail_(0)
		{
			data_ = new T[capacity_];
		}
		~threadsafe_ring() { delete[] data_; }
		threadsafe_ring(const threadsafe_ring&) = delete;
		threadsafe_ring& operator=(const threadsafe_ring&) = delete;

		int count() const { 
			std::lock_guard<std::mutex> lock(mutex_);
			return (tail_ - head_ + capacity_) % capacity_;
		}
		bool empty() const {
			std::lock_guard<std::mutex> lock(mutex_);
			return head_ == tail_;
		}
		bool full() const {
			std::lock_guard<std::mutex> lock(mutex_);
			return (tail_ + 1) % capacity_ == head_; 
		}
		std::vector<T> to_vector() const;

		bool pop(T& e);
		bool push(const T& e);

	private:
		mutable std::mutex mutex_;
		const int capacity_;
		T* data_;
		int head_;
		int tail_;
};

template<typename T>
bool threadsafe_ring<T>::pop(T& e)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if(head_ == tail_)
	{
		return false;
	}

	e = data_[head_];
	head_ = (head_ + 1) % capacity_;
	return true;
}

template<typename T>
bool threadsafe_ring<T>::push(const T& e)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if((tail_ + 1) % capacity_ == head_)
	{
		return false;
	}

	data_[tail_] = e;
	tail_ = (tail_ + 1) % capacity_;
	return true;
}

template<typename T>
std::vector<T> threadsafe_ring<T>::to_vector() const
{
	std::vector<T> v;
	std::lock_guard<std::mutex> lock(mutex_);
	for(int i = head_; i != tail_; i = (i + 1) % capacity_)
	{
		v.push_back(data_[i]);
	}
	return v;
}

#endif

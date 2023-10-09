#ifndef _RING_H_
#define _RING_H_
#include <vector>

template<typename T>
class ring
{ // ring queue
	public:
		ring(int capacity):
			capacity_(capacity),
			head_(0),
			tail_(0)
		{
			data_ = new T[capacity_];
		}
		~ring() { delete[] data_; }
		ring(const ring&) = delete;
		ring& operator=(const ring&) = delete;

		int count() const { return (tail_ - head_ + capacity_) % capacity_; }
		bool empty() const { return head_ == tail_; }
		bool full() const { return (tail_ + 1) % capacity_ == head_; }
		std::vector<T> to_vector() const;

		bool pop(T& e);
		bool push(const T& e);

	private:
		const int capacity_;
		T* data_;
		int head_;
		int tail_;
};

template<typename T>
bool ring<T>::pop(T& e)
{
	if(head_ == tail_)
	{
		return false;
	}

	e = data_[head_];
	head_ = (head_ + 1) % capacity_;
	return true;
}

template<typename T>
bool ring<T>::push(const T& e)
{
	if((tail_ + 1) % capacity_ == head_)
	{
		return false;
	}

	data_[tail_] = e;
	tail_ = (tail_ + 1) % capacity_;
	return true;
}

template<typename T>
std::vector<T> ring<T>::to_vector() const
{
	std::vector<T> v;
	for(int i = head_; i != tail_; i = (i + 1) % capacity_)
	{
		v.push_back(data_[i]);
	}
	return v;
}

#endif

#ifndef _THREADSAFE_SYS_SSTAILQ_H_
#define _THREADSAFE_SYS_SSTAILQ_H_

#include <unistd.h>
#include <sys/queue.h>
#include <memory>
#include <mutex>
#include <vector>

template<typename T>
class threadsafe_sys_stailq
{
	public:
		threadsafe_sys_stailq();
		~threadsafe_sys_stailq();

		std::unique_ptr<T> try_pop();
		void push(const T& e);

		std::vector<T> to_vector() const;
		bool empty() const;
		int count() const;

	private:
		struct node
		{
			std::unique_ptr<T> data;
			STAILQ_ENTRY(node) tailq;

			node(const T& e)
				:data(new T(e)) {}
		};
		//单向有尾链表
		STAILQ_HEAD(queue, node);

		mutable std::mutex mutex_;
		struct queue q_;
};

template<typename T>
threadsafe_sys_stailq<T>::threadsafe_sys_stailq()
{
	STAILQ_INIT(&q_);
}

template<typename T>
threadsafe_sys_stailq<T>::~threadsafe_sys_stailq()
{
	std::lock_guard<std::mutex> lock(mutex_);
	for(auto p = STAILQ_FIRST(&q_); p; p = STAILQ_FIRST(&q_))
	{
		STAILQ_REMOVE(&q_, p, node, tailq);
		delete p;
	}
}

template<typename T>
std::unique_ptr<T> threadsafe_sys_stailq<T>::try_pop()
{
	std::lock_guard<std::mutex> lock(mutex_);
	if(STAILQ_EMPTY(&q_))
	{
		return nullptr;
	}

	auto p = STAILQ_FIRST(&q_); 
	STAILQ_REMOVE(&q_, p, node, tailq);
	auto ret = std::move(p->data);
	delete p;
	return ret;
}

template<typename T>
void threadsafe_sys_stailq<T>::push(const T& e)
{
	struct node* p = new node(e);
	std::lock_guard<std::mutex> lock(mutex_);
	STAILQ_INSERT_TAIL(&q_, p, tailq);
}

template<typename T>
std::vector<T> threadsafe_sys_stailq<T>::to_vector() const
{
	std::vector<T> v;
	std::lock_guard<std::mutex> lock(mutex_);
	for(auto p = STAILQ_FIRST(&q_); p; p = STAILQ_NEXT(p, tailq))
	{
		v.push_back(*p->data);
	}
	return v;
}

template<typename T>
bool threadsafe_sys_stailq<T>::empty() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return STAILQ_EMPTY(&q_);
}

template<typename T>
int threadsafe_sys_stailq<T>::count() const
{
	int ct = 0;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		for(auto p = STAILQ_FIRST(&q_); p; p = STAILQ_NEXT(p, tailq))
		{
			ct++;
		}
	}
	return ct;
}

#endif

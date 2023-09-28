#ifndef _THREADSAFE_SINGLE_LIST_QUEUE_H_
#include <mutex>
#include <vector>
#include <memory>
#include <cassert>

template<typename T>
class threadsafe_single_list_queue
{
	public:

		using value_type = T;

		threadsafe_single_list_queue();

		threadsafe_single_list_queue(const threadsafe_single_list_queue&) = delete;
		threadsafe_single_list_queue& operator=(const threadsafe_single_list_queue&) = delete;

		std::unique_ptr<T> try_pop();
		void push(const T& e);

		//O(n)
		std::vector<T> to_vector() const;

		//O(n)
		int count() const;

		bool empty() const
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return !bool(head_);
		}

	private:
		struct node
		{
			node(const T& e)
				: data(std::make_unique<T>(e)), next(nullptr)
			{}

			node()
				:next(nullptr)
			{
			}
			
			std::unique_ptr<T> data;
			std::unique_ptr<node> next;
		};

		std::unique_ptr<node> head_;
		node* tail_;
		mutable std::mutex mutex_;
};

template<typename T>
threadsafe_single_list_queue<T>::threadsafe_single_list_queue()
		:tail_(nullptr)
{
}

/** 只有一个节点时pop与push存在竞争*/
template<typename T>
std::unique_ptr<T> threadsafe_single_list_queue<T>::try_pop()
{
	std::lock_guard<std::mutex> lock(mutex_);
	if(!head_)
	{
		return nullptr;
	}
	auto ret = std::move(head_->data);
	auto next = std::move(head_->next);
	head_ = std::move(next);
	return ret;
}

template<typename T>
void threadsafe_single_list_queue<T>::push(const T& e)
{ // push 操作访问head_和tail_两个指针
	auto new_node = std::make_unique<node>(e);
	node* new_tail = new_node.get();
	
	std::lock_guard<std::mutex> lock(mutex_);
	if(head_) // n -> (n + 1)
	{
		tail_->next = std::move(new_node);
	}
	else // 0 -> 1
	{ 
		head_ = std::move(new_node);
	}
	tail_ = new_tail;
}
		
template<typename T>
std::vector<T> threadsafe_single_list_queue<T>::to_vector() const
{
	std::vector<T> v;
	std::lock_guard<std::mutex> lock(mutex_);
	for(node* p = head_.get(); p; p = p->next.get())
	{
		v.push_back(p->data);
	}
	return v;
}
	
template<typename T>
int threadsafe_single_list_queue<T>::count() const
{
	int c = 0;
	std::lock_guard<std::mutex> lock(mutex_);
	for(node* p = head_.get(); p; p = p->next.get())
	{
		++c;
	}
	return c;
}

#endif //_THREADSAFE_QUEUE_H_

#ifndef _THREADSAFE_QUEUE_WITH_DUMMY_NODE_H_
#include <vector>
#include <memory>
#include <mutex>

template<typename T>
class threadsafe_queue_with_dummy_node
{
	public:

		using value_type = T;

		threadsafe_queue_with_dummy_node();

		threadsafe_queue_with_dummy_node(const threadsafe_queue_with_dummy_node&) = delete;
		threadsafe_queue_with_dummy_node& operator=(const threadsafe_queue_with_dummy_node&) = delete;

		std::unique_ptr<T> try_pop();
		void push(const T& e);

		////O(n)
		//std::vector<T> to_vector() const;

		////O(n)
		//int count() const;

		//bool empty() const
		//{
		//	return !bool(head_);
		//}

	private:
		struct node
		{
			node(const T& e)
				: data(std::make_unique<T>(e)), next(nullptr)
			{}

			node()
				:data(nullptr),
				next(nullptr)
			{
			}
			
			std::unique_ptr<T> data;
			std::unique_ptr<node> next;
		};

		mutable std::mutex head_mutex_;
		mutable std::mutex tail_mutex_;
		std::unique_ptr<node> head_;
		node* tail_;
};

template<typename T>
threadsafe_queue_with_dummy_node<T>::threadsafe_queue_with_dummy_node()
		:head_(new node()),
		tail_(head_.get())
{
}

template<typename T>
std::unique_ptr<T> threadsafe_queue_with_dummy_node<T>::try_pop()
{
	std::lock_guard<std::mutex> hlock(head_mutex_);
	{
		std::lock_guard<std::mutex> tlock(tail_mutex_);
		if(head_.get() == tail_)
		{
			return nullptr;
		}
	}
	auto ret = std::move(head_->data);
	auto next = std::move(head_->next);
	head_ = std::move(next);
	return ret;
}

template<typename T>
void threadsafe_queue_with_dummy_node<T>::push(const T& e)
{
	auto new_data = std::make_unique<T>(e);
	auto new_node = std::make_unique<node>(e);
	node* new_tail = new_node.get();

	std::lock_guard<std::mutex> tlock(tail_mutex_);
	tail_->data = std::move(new_data);
	tail_->next = std::move(new_node);
	tail_ = new_tail;
}
		
//template<typename T>
//std::vector<T> threadsafe_queue_with_dummy_node<T>::to_vector() const
//{
//	std::vector<T> v;
//	for(node* p = head_.get(); p; p = p->next.get())
//	{
//		v.push_back(p->data);
//	}
//	return v;
//}
	
//template<typename T>
//int threadsafe_queue_with_dummy_node<T>::count() const
//{
//	int c = 0;
//	for(node* p = head_.get(); p; p = p->next.get())
//	{
//		++c;
//	}
//	return c;
//}

#endif //_QUEUE_WITH_DUMMY_NODE_H_

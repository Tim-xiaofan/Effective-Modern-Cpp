#ifndef _QUEUE_WITH_DUMMY_NODE_H_
#include <vector>
#include <memory>

template<typename T>
class queue_with_dummy_node
{
	public:
		queue_with_dummy_node();

		queue_with_dummy_node(const queue_with_dummy_node&) = delete;
		queue_with_dummy_node& operator=(const queue_with_dummy_node&) = delete;

		std::unique_ptr<T> try_pop();
		void push(const T& e);

		//O(n)
		std::vector<T> to_vector() const;

		//O(n)
		int count() const;

		bool empty() const
		{
			return !bool(head_);
		}

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

		std::unique_ptr<node> head_;
		node* tail_;
};

template<typename T>
queue_with_dummy_node<T>::queue_with_dummy_node()
		:head_(new node()),
		tail_(head_.get())
{
}

template<typename T>
std::unique_ptr<T> queue_with_dummy_node<T>::try_pop()
{
	if(head_.get() == tail_)
	{
		return nullptr;
	}
	auto ret = std::move(head_->data);
	auto next = std::move(head_->next);
	head_ = std::move(next);
	return ret;
}

template<typename T>
void queue_with_dummy_node<T>::push(const T& e)
{
	auto new_data = std::make_unique<T>(e);
	auto new_node = std::make_unique<node>(e);
	node* new_tail = new_node.get();
	tail_->data = std::move(new_data);
	tail_->next = std::move(new_node);
	tail_ = new_tail;
}
		
template<typename T>
std::vector<T> queue_with_dummy_node<T>::to_vector() const
{
	std::vector<T> v;
	for(node* p = head_.get(); p; p = p->next.get())
	{
		v.push_back(p->data);
	}
	return v;
}
	
template<typename T>
int queue_with_dummy_node<T>::count() const
{
	int c = 0;
	for(node* p = head_.get(); p; p = p->next.get())
	{
		++c;
	}
	return c;
}

#endif //_QUEUE_WITH_DUMMY_NODE_H_

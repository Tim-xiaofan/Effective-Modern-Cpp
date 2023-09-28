#ifndef _QUEUE_H_
#include <vector>
#include <memory>

template<typename T>
class queue
{
	public:
		queue();

		queue(const queue&) = delete;
		queue& operator=(const queue&) = delete;

		std::unique_ptr<T> pop();
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
				: data(e), next(nullptr)
			{}

			node()
				:next(nullptr)
			{
			}
			
			T data;
			std::unique_ptr<node> next;
		};

		std::unique_ptr<node> head_;
		node* tail_;
};

template<typename T>
queue<T>::queue()
		:tail_(nullptr)
{
}

/** 只有一个节点时pop与push存在竞争*/
template<typename T>
std::unique_ptr<T> queue<T>::pop()
{
	if(!head_)
	{
		return nullptr;
	}
	auto ret = std::make_unique<T>(std::move(head_->data));
	auto next = std::move(head_->next);
	head_ = std::move(next);
	return ret;
}

template<typename T>
void queue<T>::push(const T& e)
{ // push 操作访问head_和tail_两个指针
	auto new_node = std::make_unique<node>(e);
	node* new_tail = new_node.get();
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
std::vector<T> queue<T>::to_vector() const
{
	std::vector<T> v;
	for(node* p = head_.get(); p; p = p->next.get())
	{
		v.push_back(p->data);
	}
	return v;
}
	
template<typename T>
int queue<T>::count() const
{
	int c = 0;
	for(node* p = head_.get(); p; p = p->next.get())
	{
		++c;
	}
	return c;
}

#endif //_QUEUE_H_

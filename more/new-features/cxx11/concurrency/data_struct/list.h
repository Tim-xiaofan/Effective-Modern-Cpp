#ifndef CONCURRENCY_DATA_STRUCT_LIST_H
#define CONCURRENCY_DATA_STRUCT_LIST_H

#include <initializer_list>
#include <iostream>

template<typename T>
class list
{
    public:
        class node
        {
            public:
            node(const T& d)
                :data(d),
                next(nullptr),
                prev(nullptr)
            {
            }

            T data;
            node* next;
            node* prev;
        };

        list()
            : head_(nullptr), tail_(nullptr)
        {}

        list(std::initializer_list<T> values)
            : head_(nullptr), tail_(nullptr)
        {
            for (const T& value : values)
            {
                push_back(value);
            }
        }

        list(const list&) = delete;
        list& operator=(const list&) = delete;

        ~list()
        {
            while(head_)
            {
                node* tmp = head_;
                head_ = head_->next;
                delete tmp;
            }
        }

        node* find(const T& d)
        {
            for(node* p = head_; p; p = p->next)
            {
                if(p->data == d)
                {
                    return p;
                }
            }
            return nullptr;
        }

        node* push_back(const T& d)
        {
            node* n = new node(d);
            return push_back(n);
        }
        
        node* push_back(node* n)
        {
            if(!tail_)
            {
                head_ = tail_ = n;
            }
            else
            {
                tail_->next = n;
                n->prev = tail_;
                tail_ = n;
            }
            return n;
        }

        node* pop_front(void)
        {
            if(!head_)
            {
                return nullptr;
            }

            node* n = head_;
            head_ = head_->next;
            if(head_)
            {
                head_->prev = nullptr;
            }
            else
            {
				// If the list is now empty, update the tail pointer.
                tail_ = nullptr;
            }

            n->next = nullptr;
            n->prev = nullptr;
            return n;
        }
	
		node* pop_back(void)
		{
			if (!tail_)
			{
				return nullptr;
			}
			node* popped = tail_;
			tail_ = tail_->prev;
			if (tail_)
			{
				tail_->next = nullptr;
			}
			else
			{
				// If the list is now empty, update the head pointer.
				head_ = nullptr;
			}
			popped->next = nullptr;
			popped->prev = nullptr;
			return popped;
		}

        void erase(node* n)
        {
            if(!head_)
            {
                return;
            }

            if(n == head_)
            {
                pop_front();
            }
            else if(n == tail_)
            {
                pop_back();
            }
            else
            {
                n->prev->next = n->next;
                n->next->prev = n->prev;
                n->prev = nullptr;
                n->next = nullptr;
            }
        }

        bool empty() const
        {
            return (head_ == nullptr && tail_ == nullptr);
        }

        void dump() const
        {
            std::cout << "{";
            for(node* p = head_; p; p = p->next)
            {
                std::cout << p->data << "<->";
            }
            std::cout << "}\n";
        }

    private:
        node* head_;
        node* tail_;
};

#endif

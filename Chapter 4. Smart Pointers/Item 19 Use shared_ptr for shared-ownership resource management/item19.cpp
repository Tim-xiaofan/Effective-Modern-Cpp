#include <iostream>
#include <memory>
#include <atomic>
#include <vector>

using std::endl;
using std::cout;
using std::cerr;
using std::shared_ptr;

class A
{
	public:
		A(void){ cout << "A constructor@" << this << endl;}
		~A(void) { cout << "A destructor@" << this <<endl;}
		A & operator=(int i)
		{
			_i = i;
			return *this;
		}
	private:
		int _i;
		friend std::ostream & operator<<(std::ostream & os, const A & a)
		{
			return (os << a._i);
		}
};

namespace X
{
	template<typename T>
	class shared_ptr
	{
		private:
			T * _ptr;
			std::atomic<int>* _rf;
			void release() noexcept
			{
				if(_rf != nullptr && --(*_rf) == 0)
				{
					cout << "x::shared_ptr delete @" << _ptr << endl;
					delete _ptr;
					delete _rf;
				}
			}
		public:
			shared_ptr(T * ptr = nullptr): _ptr(ptr), _rf(new std::atomic<int>(1))
			{
				cout << "construct sp for @" << ptr << endl;
			}
			~shared_ptr(void) { release(); }
			
			shared_ptr(const shared_ptr& sp) noexcept
			{
				_ptr = sp._ptr;
				_rf = sp._rf;
				(*_rf)++;
			}
			
			shared_ptr(shared_ptr&& sp) noexcept
			{
				_ptr = sp._ptr;
				_rf = sp._rf;
				sp._ptr = nullptr;
				sp._rf = nullptr;
			}
			
			shared_ptr & operator=(const shared_ptr & sp) noexcept
			{
				if(&sp != this)
				{
					release();
					_ptr = sp._ptr;
					_rf = sp._rf;
					++(*_rf);
				}
				return *this;
			}
			
			constexpr int count(void) noexcept
			{
				return *_rf;
			}
			
			T & operator*()
			{
				return *_ptr;
			}
	};
	
	template<typename T>
	class shared_ptr<T[]>
	{
		private:
			T*  _ptr;
			std::atomic<int>* _rf;
			void release() noexcept
			{
				if(--(*_rf) == 0)
				{
					delete [] _ptr;
					delete _rf;
				}
			}
		public:
			shared_ptr(T * ptr): _ptr(ptr), _rf(new std::atomic<int>(1)) {}
			~shared_ptr(void) { release(); }
			
			shared_ptr(const shared_ptr & sp) noexcept
			{
				_ptr = sp._ptr;
				_rf = sp._rf;
				(*_rf)++;
			}
			
			shared_ptr & operator=(const shared_ptr & sp) noexcept
			{
				if(&sp != this)
				{
					release();
					_ptr = sp._ptr;
					_rf = sp._rf;
					++(*_rf);
				}
				return *this;
			}
			
			constexpr int count(void) noexcept
			{
				return *_rf;
			}
			
			T & operator*() noexcept
			{
				return *_ptr;
			}

			T & operator[](int i) noexcept
			{
				return _ptr[i];
			}

	};
}

class Widget 
{
	public:
		Widget(){cout << "construct Widget @" << this << endl;}
		~Widget(){cout << "destruct Widget @" << this << endl;}
		void process();
};

namespace Y
{
	class Widget: public std::enable_shared_from_this<Widget>
	{
		public:
			void process();
	};
	
	std::vector<std::shared_ptr<Widget> > processWidgets;

	void Widget::process()
	{
		cout << "emplace_back @" << this << endl;
		processWidgets.emplace_back(shared_from_this()); 
	}
}

namespace Z
{
	class Widget: public std::enable_shared_from_this<Widget>
	{
		public:
			void process();
			static std::shared_ptr<Widget> create();
		private:
			Widget() = default;
	};
	
	std::vector<std::shared_ptr<Widget> > processWidgets;

	std::shared_ptr<Widget> Widget:: create()
	{
		return std::move(std::shared_ptr<Widget>(new Widget));
	}

	void Widget::process()
	{
		cout << "emplace_back @" << this << endl;
		processWidgets.emplace_back(shared_from_this()); 
	}
}

std::vector<X::shared_ptr<Widget> > processWidgets;

void Widget::process()
{
	cout << "emplace_back @" << this << endl;
	processWidgets.emplace_back(this); //add it to list of processed Widgets; 
										//this is wrong
}

template<typename T, typename ...Args>
void drop(T &t, Args&... rest)
{}

int main(int argc, char * argv[])
{
	cout << "sizeof(A*):" << sizeof(A*) << endl;
	cout << "sizeof(X::shared_ptr<A>):" << sizeof(X::shared_ptr<A>) << endl;
	cout << "sizeof(X::shared_ptr<A[]>):" << sizeof(X::shared_ptr<A[]>) << endl;
	cout << "sizeof(std::shared_ptr<A>):" << sizeof(std::shared_ptr<A>) << endl;
	cout << "sizeof(std::shared_ptr<A[]>):" << sizeof(std::shared_ptr<A[]>) << endl;

	{
		cout << "\n# implement of shared_ptr<T>" << endl;
		X::shared_ptr<A> sp(new A);
		cout << "constructor:"<< sp.count() << endl;

		X::shared_ptr<A> sp1 = sp;
		cout << "\ncopy constructor, sp:"<< sp.count() << endl;
		cout << "copy constructor,sp1:"<< sp1.count() << endl;

		sp = sp1;
		cout << "\ncopy assignment, sp:" << sp.count() << endl;
		cout << "copy assignment, sp1:" << sp1.count() << endl;

		X::shared_ptr<A> sp_(new A);
		sp = sp_;
		cout << "\ndiffcult object copy assignment, sp(new):" << sp.count() << endl;
		cout << "diffcult object copy assignment, sp_(new):" << sp_.count() << endl;
		cout << "diffcult object copy assignment, sp1(old):" << sp1.count() << endl;

		X::shared_ptr<A> sp_1(std::move(sp));
		cout << "\nmove assignment, sp_1(new):" << sp_1.count() << endl;
		cout << "move assignment, sp_(new):" << sp_.count() << endl;
		cout << "move assignment, sp1(old):" << sp1.count() << endl;
	}

	{
		cout << "\n# implement of shared_ptr<T[]>" << endl;
		X::shared_ptr<A[]> sp(new A[2]);
		cout << "constructor:"<< sp.count() << endl;

		X::shared_ptr<A[]> sp1 = sp;
		cout << "copy constructor, sp:"<< sp.count() << endl;
		cout << "copy constructor,sp1:"<< sp1.count() << endl;

		sp = sp1;
		cout << "copy assignment, sp:" << sp.count() << endl;
		cout << "copy assignment, sp1:" << sp1.count() << endl;

		sp[0] = 1;
		sp1[1] = 2;
		cout << sp[0] << endl;
		cout << sp[1] << endl;
		cout << sp[0] << endl;
		cout << sp[1] << endl;


		X::shared_ptr<A[]> sp_(new A[2]);
		sp = sp_;
		cout << "\ndiffcult object copy assignment, sp(new):" << sp.count() << endl;
		cout << "diffcult object copy assignment, sp_(new):" << sp_.count() << endl;
		cout << "diffcult object copy assignment, sp1(old):" << sp1.count() << endl;

	}

	{
		cout << "\n# shared_ptr custom deletor" << endl;
		auto deletor = [](Widget * w) 
		{cout << "delete@" << w << endl; delete w;};

		std::unique_ptr<Widget, decltype(deletor)> upw(new Widget, deletor);
		std::shared_ptr<Widget> spw(new Widget, deletor);

	}

	{
		cout << "\n# multiple control blocks : UB" << endl;
		auto pw = new Widget;
		std::shared_ptr<Widget> spw1(pw);
		//std::shared_ptr<Widget> spw2(pw); // error! double free
	}

	{
		cout << "\n# multiple control blocks: avoid" << endl;
		auto deletor = [](Widget * w) 
		{cout << "delete@" << w << endl; delete w;};
		
		std::shared_ptr<Widget> spw1(new Widget, deletor);
		std::shared_ptr<Widget> spw2(spw1);
	}

	{
		cout << "\n# lead to multiple control blocks involves "
			"the this pointer" << endl;
		Widget w;
		//w.process();
		//cout << processWidgets[0].count() << endl;
		drop(w);
	}

	{
		cout << "\n# std::enable_shared_from_this" << endl;
		std::shared_ptr<Y::Widget> spw(new Y::Widget);
		spw->process();
		cout << "spw use_count:" << spw.use_count() << endl;
	}

	{
		cout << "\n# std::enable_shared_from_this: avoid exception" << endl;
		auto spw = Z::Widget::create();
		spw->process();
		cout << "spw use_count:" << spw.use_count() << endl;
	}
	return 0;
}

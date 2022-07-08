#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <atomic>
#include <stdlib.h>
#include <mutex>
#include <list>
#include <type_traits>
#include <sys/time.h>

using std::endl;
using std::cout;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::unordered_map;
using std::string;

namespace _1
{

	class Base
	{
		public:
			virtual void doWork(){ cout << "Base::doWork" << endl;}
			virtual ~Base() {cout << "Base::~Base()" << endl;}
	};

	class Derived:public Base
	{
		public:
			virtual void doWork(){ cout << "Derived::doWork" << endl;}
			virtual ~Derived() { cout << "Derived::~Derived()" << endl;}
	};


	class Widget
	{
		public:
			void doWork() & { cout << "lvalue doWork" << endl;}
			void doWork() && { cout << "rvalue doWork " << endl;}
	};

	Widget makeWidget()
	{
		return Widget();
	}
}

namespace _2
{
	class Base
	{
		public:
			virtual void mf1() const {}
			virtual void mf2(int x) {}
			virtual void mf3() & {}
			virtual void mf4() const {}
	};

	class Derived: public Base
	{
		public:
			// virtual void mf1() override{} //error
			virtual void mf1() const override{}
			//virtual void mf2(unsigned int x) override{} // error
			virtual void mf2(int x) override{}
			//virtual void mf3() && override {} // error
			virtual void mf3() & override {}
			virtual void mf4() const override {} 
	};

	class Waring
	{
		public:
			void override(); // legal in both C++98 and C++11
	};

	class Widget
	{
		public:
			using DataType = std::vector<double>;
			
			Widget():values(10000, 20) {}

			DataType & data() & 
			{ return values;} // for lvalue Widgets, return l

			DataType data() && // for rvalue Widgets, return rvalue
			{ return std::move(values);}
		private:
			DataType values;
	};
	
	Widget makeWidget()
	{
		return Widget();
	}

	double time_diff(const struct timeval & begin, const struct timeval & end)
	{
		return (end.tv_sec + static_cast<double>(end.tv_usec) / 1000000) 
			- (begin.tv_sec + static_cast<double>(begin.tv_usec) / 1000000);
	}

	void dump_tv(const struct timeval & tv)
	{
		cout << "<" << tv.tv_sec << "," << tv.tv_usec << ">" << endl;
	}
};

template <typename C>
void dump(const C & c)
{
	for(auto p = c.begin(); p != c.end(); ++p)
	{
		cout << (*p) << " ";
	}
	cout << endl;
}

int main(int argc, char * argv[])
{
	{
		using namespace _1;
		cout << "# overriding" << endl;
		std::unique_ptr<Base> upb = std::make_unique<Derived>();
		upb->doWork();
	}

	{
		using namespace _1;
		cout << "\n# reference qualifiers" << endl;
		Widget w; // normal object(an lvalue)
		w.doWork();
		makeWidget().doWork();
		Widget w1 = makeWidget();
		w1.doWork();
		std::move(w1).doWork();
	}

	{
		cout << "\n# reference qualifiers1" << endl;
		using namespace _2;
		Widget w;
		struct timeval begin, end;
		gettimeofday(&begin, nullptr);
		for(int i = 0; i < 10000 * 15; ++i)
		{
			auto vals1 = w.data(); // copy w.values into vals1, copy ctor
		}
		gettimeofday(&end, nullptr);
		cout << "cost1:" << time_diff(begin, end) << "s"<< endl;
		
		gettimeofday(&begin, nullptr);
		for(int i = 0; i < 10000 * 15; ++i)
		{
			auto vals2 = makeWidget().data(); // move-constructor
		}
		gettimeofday(&end, nullptr);
		cout << "cost2:" << time_diff(begin, end) << "s"<< endl;
	}
	return 0;
}

#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <atomic>

using std::endl;
using std::cout;
using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::string;

template <typename C>
std::ostream & dump(const C & c, std::ostream & os = std::cout)
{
	for(auto it = c.begin(); it!= c.end(); ++it)
	  os << (*it) << " ";
	return os;
}

/**
  type of object to create
  types of arguments to use
 */
template<typename T, typename... Ts>
void doSomeWork(Ts &&... params)
{
	T localObject(std::forward<Ts>(params)...); // using parens
	dump(localObject) << endl;
	T localObject1{std::forward<Ts>(params)...}; // using braces
	dump(localObject1) << endl;
}

class Widget 
{
	public:
		Widget(int i = 0) { cout << "constructor" << endl;}
		Widget(int i, bool b) { cout << "constructor 1" << endl;} 
		Widget(int i, double d) { cout << "constructor 2" << endl;} 
		Widget(std::initializer_list<long double> il) 
		{
			cout << "constructor 3, size = " << il.size() << ", params: ";
			dump(il) << endl;
		}
		Widget(const Widget & w) { cout << "copy constructor" << endl; }
		Widget(Widget && w) { cout << "move constructor" << endl;}
		Widget & operator=(const Widget & w) { cout << "assignment" << endl; 
			return *this;}
		operator float() const{return 3.14;} // convert to float
};


int main(int argc, char * argv[])
{
	{
		cout << "# C++11 initializer" << endl;
		int x(0); // initializer is in parentheses
		int y = 0; // initializer follows "="
		int z{0}; // initializer is in braces
		int i = {0}; // initializer uses "=" and braces
		cout << x << y << z << endl;
		cout << i << endl;
	}

	{
		cout << "\n# Distinguish initialization from assignment" << endl;
		Widget w1; // call default constructor
		Widget w2 = w1; // not an assignment; calls copy ctor
		w1 = w2; // an assignment; calls copy operator=
	}

	{
		cout << "\n# STL container initialization" << endl;
		vector<int> vi = {1, 3, 5};
		dump(vi) << endl;
	}

	{
		cout << "\n# Default initialization values for non-static data" << endl;
		struct Widget1
		{
			int x{-1};
			int y = 0;
			int z = 99;
		};
		struct Widget1 w;
		cout << "w.x:" << w.x << endl;
		cout << "w.y:" << w.y << endl;
		cout << "w.z:" << w.z << endl;
	}

	{
		cout << "\n# Uncopyable objects" << endl;
		std::atomic<int> ai1{0}; //fine
		std::atomic<int> ai2(0); //fine
		//std::atomic<int> ai3 = 0; //error
	}

	{
		cout << "\n# Prohibits implicit narrowing conversions among built-in types" 
			<< endl;
		double x = 1.5, y = 2.5, z = 9.8;
		
		int sum0 = x + y + z;
		cout << "sum0:" << sum0 << endl;
		
		int sum1{ x + y + z}; //gcc version 9.4.0, warnning
		cout << "sum1:" << sum1 << endl;

		int sum2(x + y + z); // okay (value of expression truncated to an int)
		cout << "sum2:" << sum2 << endl;

	}

	{
		cout << "\n# Avoid most vexing parse" << endl;
		Widget w1(10); // call Widget ctor with argument 10
		Widget w2(); // most vexing parse! declares a function,  
		            //named w2 that returns a Widget!
		Widget w3{}; // calls Widget ctor with no args
	}

	{
		cout << "\n# std::initializer_list hijacked construction and move construction"
			<< endl;
		Widget w1(10, true); // calls first ctor
		Widget w2{10, true}; // also calls first ctor
		Widget w3(10, 5.0); // calls second ctor
		Widget w4{10, 5.0}; // also calls second ctor

		Widget w5(w4); // uses parenthesis, call copy ctor
		Widget w6{w4}; // uses {} , call std::initializer_list 
						//ctor(w4-->float-->long double)
		Widget w7(std::move(w4)); //use (), call move ctor
		Widget w8{std::move(w4)};
	}

	{
		cout << "\n# An edge case" << endl;
		class Widget1
		{
			public:
				Widget1(){ cout << "constructor" << endl;}
				Widget1(std::initializer_list<int> il){ 
					cout << "std::initializer_list constructor" << endl;}
		};
		Widget1 w1; // calls default ctor
		Widget1 w2{}; // also calls default ctor
		Widget1 w3(); // most vexing parse! declares a function!
		Widget1 w4({}); // calls std::initializer_list ctor
		Widget1 w5{{}}; // with empty list
	}

	{
		cout << "\n# std::vector" << endl;
		vector<int> vi(10, 20); // use non-std::initializer_list ctor: create 10-element
		dump(vi) << endl;

		vector<int> vi1{10, 20};
		dump(vi1) << endl;

		doSomeWork<std::vector<int> >(10, 20);
	}
	return 0;
}

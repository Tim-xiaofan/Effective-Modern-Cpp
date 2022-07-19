#include <iostream>
#include <string>
#include <memory>
#include <set>
#include <atomic>
#include <vector>

using std::endl;
using std::cout;
using std::cerr;

namespace X
{
	class string
	{
		public:
			string():_id(++id)
			{
				//cout << "string::default constructor" << endl;
			}
			
			string(const char* str):_id(++id)
			{
				cout << "string::construct from const char*" << endl;
			}

			string(const string& rhs)
			{
				_id = rhs.id;
				cout << "string::copy constructor" << endl;
			}

			string(string&& rhs)
			{
				_id = rhs.id;
				rhs.id = 0;
				cout << "string::move constructor" << endl;
			}

			constexpr bool operator<(const string& rhs) { return (_id < rhs._id);}
		private:
			static std::atomic<int> id;
			int _id;
	};

	std::atomic<int> string::id = {0};
}

std::multiset<X::string> names;

void logAndAdd(const X::string& name)
{
	names.emplace(name);
}

static std::vector<X::string> names1(32);
X::string nameFromIdx(int idx)
{
	return names1[idx];
}

template<typename T>
void logAndAdd1(T&& name)
{
	names.emplace(std::forward<T>(name));
}

void logAndAdd1(int idx)
{
	names.emplace(nameFromIdx(idx));
}

class Person
{
	public:
		template<typename T>
		explicit Person(T&& n) // ctor1
			:name(std::forward<T>(n)){}

		explicit Person(int idx) //ctor2
			:name(nameFromIdx(idx)){}
	private:
		X::string name;
};

template<typename... Args>
void drop(Args ...args)
{
}

int main(int argc, char * argv[])
{
	{
		cout << "\n# There’s no way to avoid that copy" << endl;
		X::string petName("Darla");
		logAndAdd(petName); // pass lvalue std::string
		cout << endl;

		logAndAdd(X::string("Persephone")); // pass rvalue std::string
		cout << endl;

		logAndAdd("Patty Dog"); // pass string literal
	}

	{
		cout << "\n\n# copy and move" << endl;
		X::string petName("Darla");
		logAndAdd1(petName); // pass lvalue std::string
		cout << endl;

		logAndAdd1(X::string("Persephone")); // pass rvalue std::string
		cout << endl;

		logAndAdd1("Patty Dog"); // pass string literal
		cout << endl;

		logAndAdd1(22);
		cout << endl;

		short idx = 21;
		//logAndAdd(idx); // error!
		drop(idx);
	}

	{
		cout << "\n\n# Worse" << endl;
		Person p("Nancy");
		//auto cloneOfP(p); // error! It will call the perfect forwarding constructor
		drop(p);
	}
	return 0;
}

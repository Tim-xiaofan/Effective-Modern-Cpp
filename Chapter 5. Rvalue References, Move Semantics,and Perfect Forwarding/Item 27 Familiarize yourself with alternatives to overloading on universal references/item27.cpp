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

static std::vector<X::string> names1(32);
X::string nameFromIdx(int idx)
{
	return names1[idx];
}

template<typename T>
void logAndAddImpl(T&& name, std::false_type)
{
	names.emplace(std::forward<T>(name));
}

template<typename T>
void logAndAddImpl(T&& idx, std::true_type)
{
	names.emplace(nameFromIdx(idx));
}

template<typename T>
void logAndAdd(T&& name)
{
	logAndAddImpl(std::forward<T>(name), 
				std::is_integral<typename std::remove_reference<T>::type>());
}

class Person
{
	public:
		explicit Person(X::string n) // replace T&& ctor
			:name(std::move(n)){}

		explicit Person(int idx) //ctor2
			:name(nameFromIdx(idx)){}
	private:
		X::string name;
};

class Person1
{
	public:
		Person1() = default;
		template<
			typename T, 
			typename=typename std::enable_if<
				!std::is_base_of<Person1, 
							 typename std::decay<T>::type
							 >::value
				&&
				!std::is_integral<typename std::remove_reference<T>::type>::value
				>::type>
		explicit Person1(T&& n) // forward ctor
			:name(std::forward<T>(n)){ cout << "Person1::forward ctor" << endl;}

		explicit Person1(int idx) //ctor2
			:name(nameFromIdx(idx)){ cout << "Person1:: construct from int" << endl;}

		Person1(const Person1& p) { cout << "Person1::copy constructor" << endl;}
		
		Person1(Person1&& p) { cout << "Person1::move constructor" << endl;}

	private:
		X::string name;
};


class SpecialPerson: public Person1
{
	public:
		SpecialPerson() = default;
		SpecialPerson(const SpecialPerson& rhs): Person1(rhs){} //call base forwarding ctor
		SpecialPerson(SpecialPerson&& rhs):Person1(std::move(rhs)){} //call base forwarding ctor
};

template<typename... Args>
void drop(Args&& ...args)
{
}

int main(int argc, char * argv[])
{
	{
		cout << "# pass by value" << endl;
		short s = 1;
		int i = 0;
		
		Person p("Nancy");
		cout << endl;
		
		Person ps(s);
		cout << endl;
		
		Person pi(i);
		cout << endl;
	}

	{
		cout << "\n# Use Tag dispatch" << endl;
		logAndAdd("Nancy");
		cout << endl;
		
		logAndAdd(22);
		cout << endl;
	}

	{
		cout << "\n# std::is_same and std::decay" << endl;
		cout << "Person = Person&: " << std::boolalpha<< 
			std::is_same<Person, Person&>::value << endl;
		
		cout << "Person = const Person: " << std::boolalpha<< 
			std::is_same<Person, const Person>::value << endl;
		
		cout << "Person = volatile Person: " << std::boolalpha<< 
			std::is_same<Person, volatile Person>::value << endl;
		
		cout << "Person = std::decay<Person&>::type: " << std::boolalpha<< 
			std::is_same<Person, typename std::decay<Person&>::type>::value << endl;

		cout << "Person = std::decay<const Person>::type: " << std::boolalpha<< 
			std::is_same<Person, typename std::decay<const Person>::type>::value << endl;

		cout << "Person = std::decay<volatile Person>::type: " << std::boolalpha<< 
			std::is_same<Person, typename std::decay<volatile Person>::type>::value << endl;

		cout << "Person = std::decay<volatile Person&>::type: " << std::boolalpha<< 
			std::is_same<Person, typename std::decay<volatile Person&>::type>::value << endl;
	}

	{
		cout << "\n# std::enable_if" << endl;
		Person1 p("Nancy");
		Person1 p1 = p;
		cout << endl;
		
		const Person1 p2("Nancy");
		Person1 p3 = p2;
		cout << endl;

		
		drop(p, p1, p2, p3);
	}

	{
		cout << "\n# std::is_base_of" << endl;
		cout << "iostream is derived from ostream: " << std::boolalpha 
			<< std::is_base_of<std::ostream, std::iostream>::value 
			<< endl;
		
		cout << "ostream is derived from ostream: " << std::boolalpha 
			<< std::is_base_of<std::ostream, std::ostream>::value 
			<< endl;

		cout << "SpecialPerson is derived from Person1: " << std::boolalpha
			<< std::is_base_of<Person1, SpecialPerson>::value
			<< endl;
		
		cout << "int is derived from int: " << std::boolalpha
			<< std::is_base_of<int, int>::value 
			<< endl;

		cout << endl;
		SpecialPerson sp1;
		SpecialPerson sp2=sp1;
		
		drop(sp1, sp2);
	}

	{
		cout << "\n# std::is_integral" << endl;
		short idx = 22;
		cout << "short is integral:" << std::boolalpha << 
			std::is_integral<short>::value << endl;
		Person1 p(idx);
		drop(p);
	}
	return 0;
}

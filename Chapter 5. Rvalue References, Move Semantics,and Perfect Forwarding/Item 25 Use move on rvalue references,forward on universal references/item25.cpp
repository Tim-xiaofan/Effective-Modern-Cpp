#include <iostream>
#include <string>
#include <memory>

using std::endl;
using std::cout;
using std::cerr;

void type_info(const char*)
{
	cout << "const char *" << endl;
}

void type_info(std::string)
{
	cout << "std::string" << endl;
}

class Widget
{
	public:
		Widget(const std::string& mname, const std::string& pstr)
			:name(mname), p(std::make_shared<std::string>(pstr)){}
		
		Widget(Widget&& rhs)
			:name(std::move(rhs.name)), p(std::move(rhs.p))
		{ cout << "Widget:: move constructor" << endl; }

		template<typename T>
		void setName(T&& newName)
		{
			type_info(newName);
			name = std::forward<T>(newName);
		}

		template<typename T>
		void setName1(T&& newName)
		{
			name = std::move(newName); // bad
		}

		friend std::ostream& operator<<(std::ostream& os, const Widget & w)
		{
			os << w.name;
			return os;
		}

	private:
		std::string name;
		std::shared_ptr<std::string> p;
};

/** overload doesn't work*/
template<typename... Args>
void drop(Args&& ...args)
{}

class Matrix 
{
	public:
		Matrix()=default;
		
		Matrix(const Matrix & rhs)
		{
			cout << "Matrix::copy constructor" << endl;
		}

		Matrix& operator+=(const Matrix & rhs) {return *this;}
};

Matrix operator+(Matrix&& lhs, const Matrix& rhs)
{
	lhs+=rhs;
	return std::move(lhs);
}

struct C 
{
	C() { cout << "C::constructor" << endl; }
	C(const C&) { std::cout << "A copy was made.\n"; }
	//C(C&&) { std::cout << "A move was made.\n"; }
};

C f_copy() 
{
	C c;
	cout << __func__ << " is called" << endl;
	return c;
}

C f_move() 
{
	C c;
	cout << __func__ << " is called" << endl;
	return std::move(c); // bad! prevents copy elision
}


int main(int argc, char * argv[])
{
	{
		cout << "# std::forward on universal references" << endl;

		Widget w1("w1", "");
		cout << "w1:" << w1 << endl;
		
		Widget w2(std::move(w1));
		cout << "w1:" << w1 << endl;
		cout << "w2:" << w2 << endl;

		w2.setName("w2_new");
		cout << "w2:" << w2 << endl;
	}

	{
		cout << "\n# std::move on universal references" << endl;

		Widget w("w", "");
		cout << "w:" << w << endl;
		std::string name = "name";
		w.setName1(name);
		cout << "name:" << name << endl;
	}

	{
		cout << "\n# universal references vs overload" << endl;
	}

	{
		cout << "\n# using std::move in return statement" << endl;
		Matrix A, B;
		//A + B;
		drop(A, B);
	}

	{
		cout << "\n# RVO" << endl;
		C c = f_copy();
		C c1 = f_move();
		drop(c, c1);
	}
	return 0;
}

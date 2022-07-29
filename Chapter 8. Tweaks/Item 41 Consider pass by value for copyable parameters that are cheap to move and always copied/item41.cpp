#include <iostream>
#include <vector>
#include <string>
#include <memory>

using std::endl;
using std::cout;
using std::cerr;

class Widget
{// two functions
	public:
		void addName(const std::string& newName)
		{
			names.push_back(newName);
		}

		void addName(std::string&& newName)
		{
			names.push_back(newName);
		}

	private:
		std::vector<std::string> names;
};

class Widget1
{// one: lead to bloated header files
	public:
		template<typename T>
		void addName(T&& newName)
		{
			names.push_back(std::forward<T>(newName));
		}

	private:
		std::vector<std::string> names;
};

class Widget2
{// pass by value
	public:
		void addName(std::string newName)
		{
			names.push_back(std::move(newName));
		}
		
		void setPtr(std::unique_ptr<std::string>&& ptr)
		{
			p = std::move(ptr);
		}

	private:
		std::vector<std::string> names;
		std::unique_ptr<std::string> p;
};

class Password
{
	public:
		explicit Password(std::string pwd)
			:text(std::move(pwd)){}

		void changeTo(std::string newPwd)
		{// two dynamic memory management actions within changeTo
			text = std::move(newPwd);
		}

	private:
		std::string text;
};

class SpecialWidget: public Widget
{
};

void processWidget(Widget w)
{
}

int main(int argc, char *argv[])
{
	{
		cout << "slice problem" << endl;
		SpecialWidget w;
		processWidget(w); // processWidget sees a Widget, 
						//not a SpecialWidget
	}
	return 0;
}

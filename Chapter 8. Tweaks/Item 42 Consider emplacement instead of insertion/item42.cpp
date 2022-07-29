#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstring>
#include <regex>

using std::endl;
using std::cout;
using std::cerr;

namespace X
{
	class string
	{
		public:

			string(void) noexcept
				:_data(nullptr), _len(0)
			{
				cout << "string::constructor @" << this << endl;
			}

			~string(void) noexcept
			{
				//cout << "delete @" << reinterpret_cast<void*>(_data) << endl;
				delete [] _data;
			}
			
			string(const char* s) noexcept
			{
				cout << "string::constructor with string literal @" << this << endl;
				_len = strlen(s);
				_data = new char[_len + 1];
				std::strcpy(_data, s);
			}

			string(const string& s) noexcept
			{
				cout << "string::copy constructor from @"<< &s << " to @" << this << endl;
				_len = s._len;
				_data = new char[_len + 1]; 
				std::strcpy(_data, s._data);
			}

			string(string&& s) noexcept
			{
				_len = s._len;
				_data = s._data;
				s._data = nullptr;
				cout << "string::move constructor from @"<< &s << " to @" << this << endl;
			}

			string& operator=(const string& s) noexcept
			{
				if(this != &s)
				{
					cout << "string::copy assignment from @"<< &s << " to @" << this << endl;
					_len = s._len;
					delete [] _data;
					std::strcpy(_data, s._data);
				}
				return *this;
			}

			string& operator=(string&& s) noexcept
			{
				if(this != &s)
				{
					cout << "string::move assignment from @"<< &s << " to @" << this << endl;
					_len = s._len;
					delete [] _data;
					_data = s._data;
					s._data = nullptr;
				}
				return *this;
			}

			friend std::ostream &operator<<(std::ostream &os, const string& s)
			{
				return os << s._data;
			}

		private:
			char* _data;
			std::size_t _len;
	};
}

class Widget {};

int main(int argc, char *argv[])
{
	{
		cout << "# push_back vs emplace_back" << endl;
		std::vector<X::string> vs; // container of std::string
		
		cout << "push_back" << endl;
		vs.push_back("xyzzy"); // add string literal: construct and move
		
		cout << "\nemplace_back" << endl;
		vs.emplace_back("xyzzy"); // vector grow, call move constructor
	}

	{
		cout << "\n\n# constructed into the container, not assigned" << endl;
		std::vector<X::string> vs = {"xyxxy"};
		cout << vs.size() << ": " << vs[0] << endl;
		
		cout << "\nemplace at begin" << endl;
		vs.emplace(vs.begin(), "xyxxy1");
		cout << vs.size() << ": " << vs[0] << endl;

		cout << "\nvs[0] = \"xyxxy2\"" << endl;
		vs[0] = "xyxxy2";
		cout << vs.size() << ": " << vs[0] << endl;
	}

	{
		cout << "\n\n# Avoid resource leak" << endl;
		std::vector<std::shared_ptr<Widget>> ptrs;

		auto del = [](Widget*){};
		std::shared_ptr<Widget> spw(new Widget, del), spw1(new Widget, del);
		ptrs.push_back(std::move(spw));
		ptrs.emplace_back(std::move(spw1));
	}

	{
		cout << "\n\n# explicit constructors" << endl;
		//std::regex r1(nullptr);
		std::vector<std::regex> regexes;
#ifdef ERROR1
		regexes.emplace_back(nullptr); // complie but core dump
#endif
#ifdef ERROR2
		regexes.push_back(nullptr); // error! won't compile
#endif
	}

	return 0;
}

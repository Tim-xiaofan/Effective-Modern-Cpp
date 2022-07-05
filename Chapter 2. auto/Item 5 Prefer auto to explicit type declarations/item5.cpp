#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>

using std::endl;
using std::cout;
using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::string;

class Widget 
{
    public:
        bool operator<(const Widget & w){ return false;}
};

int main(int argc, char * argv[])
{
    {
        cout << "# auto variables have their type "
            "deduced from their initializer" << endl;
        int x1;
        //auto x2; // error! initializer required
        auto x3 = 0;
        cout << "x1:" << x1 << endl;
        cout << "x3:" << x3 << endl;
    }

    {
        cout << "\n# auto can represent types known "
            "only to compilers" << endl;
		unique_ptr<Widget> p1, p2;
        auto derefUPLess = [](const unique_ptr<Widget> & p1, 
                    const unique_ptr<Widget> & p2)
        {return *p1 < *p2;};
		cout << std::boolalpha<< derefUPLess(p1, p2) << endl;
        //c++14
        auto derefLess = [](const auto & p1, const auto & p2)
        {return *p1 < *p2;};
		cout << std::boolalpha << derefLess(p1, p2) << endl;

		int i = 1, *pi1 = &i, *pi2 = &i;
		cout << std::boolalpha << derefLess(pi1, pi2) << endl;

		std::function<bool (const unique_ptr<Widget> &, const unique_ptr<Widget> &)>
			derefUPLess1 = derefUPLess;
		
		cout << std::boolalpha << derefLess(pi1, pi2) << endl;
		cout << "sizeof(std::function):" << sizeof(derefUPLess1) << endl;
		cout << "sizeof(auto):" << sizeof(derefUPLess) << endl;
    }

	{
		cout << "\n# Avoiding type shortcuts" << endl;
		vector<int> vi = {0, 1, 2, 3};
		unsigned sz = vi.size();
		auto sz1 = vi.size();
		cout << "sizeof(sz):" << sizeof(sz) << endl;
		cout << "sizeof(sz1):" << sizeof(sz1) << endl;
		cout << "sizeof(vi.size()):" << sizeof(vi.size()) << endl;

		unordered_map<string, int> m = {{"A", 0}, {"B", 1}, {"C", 2}};
		for(const std::pair<string, int> & p : m)
		{// Will create temporary object
			cout << "("<< p.first << "," << p.second << ")" << endl;
		}

		for(const std::pair<const string, int> & p : m)
		{//Ok
			cout << "("<< p.first << "," << p.second << ")" << endl;
		}

		for(const auto& p : m)
		{//Ok
			cout << "("<< p.first << "," << p.second << ")" << endl;
		}
	}
    return 0;
}

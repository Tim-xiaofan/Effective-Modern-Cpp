#include <iostream>
#include <vector>
#include <array>

using std::endl;
using std::cout;
using std::cerr;

class Widget
{
	public:
		Widget()=default;
		Widget(const Widget&) { cout << "Widget::copy constructor" << endl;}
		Widget(Widget&&) { cout << "Widget::move constructor" << endl;}
		Widget& operator=(const Widget&) 
			{ cout << "Widget::copy assignment" << endl; return *this;}
		Widget& operator=(Widget&&) 
		{ cout << "Widget::move assignment" << endl; return *this;}
};

int main(int argc, char * argv[])
{
	{
		cout << "# std::move and std::vector(not std::array)" << endl;
		std::vector<Widget> vw1(4), vw2(4);
		cout << "*construct" << endl;
		cout << "data1:"<<vw1.data() << endl;
		cout << "data2:"<<vw2.data() << endl << endl;

		cout << "*copy assignment" << endl;
		vw2 = vw1;
		cout << "data1:"<<vw1.data() << endl;
		cout << "data2:"<<vw2.data() << endl << endl;

		vw2 = std::move(vw1);
		cout << "*move assignment" << endl;
		cout << "data1:"<<vw1.data() << endl;
		cout << "data2:"<<vw2.data() << endl;
	}

	{
		cout << "\n#std::move and std::array" << endl;
		std::array<Widget, 4> aw1, aw2;

		cout << "*copy assignment" << endl;
		aw2 = aw1;

		cout << "*move assignment" << endl;
		aw2 = std::move(aw1);
	}
	return 0;
}

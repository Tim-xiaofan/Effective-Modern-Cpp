#include <iostream>

#include "widget.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	{
		Widget w1;
		Widget w2 = std::move(w1);
		Widget w3;
		w1 = std::move(w2);
		w1 = std::move(w3);
		cout << endl;
	}
	return 0;
}

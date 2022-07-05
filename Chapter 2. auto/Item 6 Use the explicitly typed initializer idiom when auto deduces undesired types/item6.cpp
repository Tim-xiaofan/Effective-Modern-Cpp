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

class Widget{};

vector<bool> features(const Widget& w)
{
	return {true, false, true, false, true, true};
}

void processWidget(const Widget & w, bool highPriority)
{
	cout << std::boolalpha << highPriority << endl;
}

double calcEpsilon(){ return 1.0; }

int main(int argc, char * argv[])
{
	Widget w;
	bool highPriority = features(w)[5]; // is w high priority?
	processWidget(w, highPriority);
	auto highPriority1 = static_cast<bool>(features(w)[5]);
	processWidget(w, highPriority1);// UB
	cout << "sizeof(false):"<< sizeof(false) << endl;

	float ep = static_cast<float>(calcEpsilon());
	cout << "ep:" << ep << endl;
    return 0;
}

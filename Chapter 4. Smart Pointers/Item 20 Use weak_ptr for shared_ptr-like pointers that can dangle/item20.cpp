#include <iostream>
#include <memory>
#include <atomic>
#include <vector>
#include <exception>
#include <unordered_map>
#include <string>

using std::endl;
using std::cout;
using std::cerr;
using WidgetID = int;

class Widget 
{
	public:
		Widget() { cout << "Widget::constructor" << endl;}
		~Widget() { cout << "Widget::destructor" << endl;}
};

std::unique_ptr<const Widget> loadWidget(WidgetID id)
{
	return std::unique_ptr<const Widget>(new Widget);
}

std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
{
	static std::unordered_map<WidgetID, 
		std::weak_ptr<const Widget>> cache;
	std::shared_ptr<const Widget> objPtr = cache[id].lock();
	if(objPtr == nullptr)
	{
		cout << "cache miss @" << id << endl; 
		objPtr = loadWidget(id);
		cache[id] = objPtr;
	}
	else
	{
		cout << "cache hit @" << id << endl;
	}
	return objPtr;
}

int main(int argc, char * argv[])
{
	{
		cout << "# test std::weak_ptr if expired" << endl;
		auto spw = std::make_shared<Widget>();
		std::weak_ptr<Widget> wpw(spw);
		
		spw = nullptr;
		if(wpw.expired())
		{
			cout << "expired" << endl;
		}
		
		std::shared_ptr<Widget> spw2 = wpw.lock();
		if(spw2 == nullptr)
		{
			cout << "spw2 == nullptr" << endl;
		}

		try
		{
			std::shared_ptr<Widget> spw3(wpw);
		}
		catch(std::exception & e)
		{
			cerr << e.what() << endl;
		}
		cout << "leave block" << endl;
	}

	{
		cout << "\n# how std::weak_ptrs can be useful : cache" << endl;
		std::shared_ptr<const Widget> spcw = fastLoadWidget(0);
		cout << "use_count:" << spcw.use_count() << endl;
		cout << endl;
		std::shared_ptr<const Widget> spcw1 = fastLoadWidget(0);
		cout << "use_count 1:" << spcw1.use_count() << endl;
	}

	{
		cout << "\n# std::shared_ptr cycle: source leak" << endl;
		struct node
		{
			node(){ cout << "node::constructor" << endl;}

			~node() {cout << "node::destructor" << endl;}

			std::shared_ptr<node> prev, next;
		};

		std::shared_ptr<node> nd(new node), nd1(new node); 
		nd->next = nd1;
		nd1->prev = nd;
	}

	{
		cout << "\n# std::shared_ptr cycle: address by std::weak_ptr" << endl;
		struct node
		{
			node(){ cout << "node::constructor" << endl;}

			~node() {cout << "node::destructor" << endl;}

			std::shared_ptr<node> next;
			std::weak_ptr<node> prev;
		};

		std::shared_ptr<node> nd(new node), nd1(new node); 
		nd->next = nd1;
	}

	{
		cout << "\n# std::weak_ptrs to break prospective cycles "
			"of std::shared_ptrs is not terribly common" << endl;
		struct node
		{
			node(){ cout << "node::constructor@" << this << endl;}
			
			node(const std::string & d):data(d)
			{ 
				cout << "node::constructor " << data << endl;
			}

			~node() {cout << "node::destructor " <<  data << endl;}

			std::unique_ptr<node> lchild{nullptr}, rchild{nullptr};
			node * parent{nullptr};
			std::string data{""};
		};

		std::unique_ptr<node> root = 
			std::unique_ptr<node>(new node("root"));
		root->lchild = 
			std::unique_ptr<node>(new node("lchild"));
		root->rchild =
			std::unique_ptr<node>(new node("rchild"));
		root->lchild->parent = root.get();
		root->rchild->parent = root.get();
		cout << endl;
	}
	return 0;
}

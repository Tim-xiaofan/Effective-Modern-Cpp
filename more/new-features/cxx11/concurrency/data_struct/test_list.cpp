#include <iostream>
#include <cassert>
#include <vector>
#include "list.h"

namespace
{
//template<typename T>
//std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
//{
//    os << "{";
//    for(const T& e: v)
//    {
//        os << e << ", ";
//    }
//    return os << "}";
//}
}

int main() 
{
    {
        list<int> mylist{1, 2, 3};
        assert(!mylist.empty());
        list<int>::node* to_erase = mylist.find(2);
        assert(to_erase);
        assert(to_erase->data == 2);
        mylist.erase(to_erase);
        assert(mylist.find(2) == nullptr);
        const list<int>::node *p = nullptr;
        std::vector<int> v;
        while((p = mylist.pop_back()))
        {
            v.push_back(p->data);
        }
        assert(v == std::vector<int>({3, 1}));
        assert(mylist.empty());
    }

    {
        list<int> mylist{1, 2, 3};
        list<int>::node* to_erase = mylist.find(1);
        assert(to_erase);
        assert(to_erase->data == 1);
        mylist.erase(to_erase);

        const list<int>::node *p = nullptr;
        std::vector<int> v;
        while((p = mylist.pop_front()))
        {
            v.push_back(p->data);
        }
        assert(v == std::vector<int>({2, 3}));

        mylist.push_back(1);
        assert(mylist.find(1));
        assert(mylist.find(1)->data == 1);
        assert(!mylist.empty());
    }

    list<int> mylist{};
    // Push elements into the list
    mylist.push_back(1);
    mylist.push_back(2);
    mylist.push_back(3);

    // Test find
    list<int>::node* found = mylist.find(2);
    assert(found != nullptr);
    assert(found->data == 2);

    // Test pop_front
    list<int>::node* popped = mylist.pop_front();
    assert(popped != nullptr);
    assert(popped->data == 1);

    // Test pop_back
    popped = mylist.pop_back();
    assert(popped != nullptr);
    assert(popped->data == 3);

    // Test erase
    list<int>::node* to_erase = mylist.find(2);
    mylist.erase(to_erase);
    assert(mylist.find(2) == nullptr);

    std::cout << "All tests passed!" << std::endl;

    return 0;
}


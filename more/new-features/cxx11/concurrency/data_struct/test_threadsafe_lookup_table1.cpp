#include <iostream>
#include <random>
#include <string>
#include <cassert>
#include <fstream>
#include "threadsafe_lookup_table1.h"
#include "test_lookup_table_utils.h"

namespace
{

void test_threadsafe_lookup_table1(void)
{
    threadsafe_lookup_table1<int> t;
    assert(t.empty());
    assert(!t.find(1));
    
    t.insert(1);
    assert(t.find(1));
    assert(t.size() == 1);
    
    t.insert(1);
    assert(t.find(1));
    assert(t.size() == 1);
    
    t.insert(2);
    assert(t.find(1));
    assert(t.find(2));
    assert(t.size() == 2);
    
    t.erase(2);
    t.erase(1);
    assert(!t.find(2));
    assert(t.size() == 0);
    assert(t.empty());
}

void mode_test(const std::vector<std::int64_t>& keys, bool reserve = false)
{
	threadsafe_lookup_table1 <std::int64_t> t;
    if(reserve) t.reserve(keys.size());
	measurements m;
	for(std::int64_t k: keys)
	{
		t.insert(k);
	}
}

}

int main(int argc, char* argv[])
{
    test_threadsafe_lookup_table1();
    
    if(argc != 3)
	{
		std::cerr << "Usage "<< argv[0] << " <num_keys>\n";
		exit(-1);
	}
	const std::int64_t num_keys = std::stoll(argv[1]);
	const std::string mode(argv[2]);

	if(mode == "random-shuffle-inserts")
	{
		auto keys = get_random_shuffle_range_ints(num_keys);
		mode_test(keys);
	}
	else if(mode == "random-full-inserts")
	{
		auto keys = get_random_full_ints(num_keys);
		mode_test(keys);
	}
    else if(mode == "random-full-reserve-inserts")
    {
		auto keys = get_random_full_ints(num_keys);
		mode_test(keys, true);
    }
    else if(mode == "random-full-delete")
    {
        auto keys = get_random_full_ints(num_keys);
	    threadsafe_lookup_table1<std::int64_t> t;
        for(std::int64_t k: keys)
        {
            t.insert(k);
        }
        std::shuffle(keys.begin(), keys.end(), generator);
        measurements m;
        for(auto key: keys)
        {
            t.erase(key);
        }
    }
    else if(mode == "random-full-read")
    {
        auto keys = get_random_full_ints(num_keys);
	    threadsafe_lookup_table1<std::int64_t> t;
        for(std::int64_t k: keys)
        {
            t.insert(k);
        }
        std::shuffle(keys.begin(), keys.end(), generator);
        measurements m;
        for(auto key: keys)
        {
            t.insert(key);
        }
    }
    else if(mode == "random-full-read-miss")
    {
        auto keys = get_random_full_ints(num_keys);
	    threadsafe_lookup_table1<std::int64_t> t;
        for(std::int64_t k: keys)
        {
            t.insert(k);
        }
        keys = get_random_full_ints(num_keys);
        measurements m;
        for(auto key: keys)
        {
            t.find(key);
        }
    }
    else if(mode == "random-full-read-half")
    {
        auto keys = get_random_full_ints(num_keys);
	    threadsafe_lookup_table1<std::int64_t> t;
        for(std::int64_t k: keys)
        {
            t.insert(k);
        }
        // delete half
        std::shuffle(keys.begin(), keys.end(), generator);
        for(int i = 0; i < num_keys / 2; ++i)
        {
            t.erase(keys[i]);
        }
        
        std::shuffle(keys.begin(), keys.end(), generator);
        measurements m;
        for(auto key: keys)
        {
            t.find(key);
        }
    }
    else if(mode == "random-full-iteration")
    {
        auto keys = get_random_full_ints(num_keys);
	    threadsafe_lookup_table1<std::int64_t> t;
        for(std::int64_t k: keys)
        {
            t.insert(k);
        }

        measurements m;
        //for(auto it = t.begin(); it != t.end(); ++it)
        //{
        //    auto k = *it;
        //    (void)k;
        //}
    }
	else
	{
		std::cerr << "unkown mode: " << mode << std::endl;
		return -1;
	}
    return 0;
}

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <unistd.h> // getpagesize
#include <unordered_set>
#include <string>

namespace
{

static std::random_device SEED{};
static std::mt19937_64 generator(SEED());

/**
 * Generate a vector [0, nb_ints) and shuffle it
 */
std::vector<std::int64_t> get_random_shuffle_range_ints(std::size_t nb_ints) {
    std::vector<std::int64_t> random_shuffle_ints(nb_ints);
	// fill with sequentially increasing values starting with value
    std::iota(random_shuffle_ints.begin(), random_shuffle_ints.end(), 0); 
    std::shuffle(random_shuffle_ints.begin(), random_shuffle_ints.end(), generator);
    
    return random_shuffle_ints;
}

/**
 * Generate random vector of random ints between min and max.
 */
std::vector<std::int64_t> get_random_full_ints(std::size_t nb_ints, 
                                               std::int64_t min = 0, 
                                               std::int64_t max = std::numeric_limits<std::int64_t>::max()) 
{
    std::uniform_int_distribution<std::int64_t> rd_uniform(min, max);
    
    std::vector<std::int64_t> random_ints(nb_ints);
    for(std::size_t i = 0; i < random_ints.size(); i++) {
        random_ints[i] = rd_uniform(generator);
    }
    
    return random_ints;
}

std::size_t get_memory_usage_bytes() {
    std::ifstream file("/proc/self/statm");

    std::size_t memory;
    file >> memory; // Ignore first
    file >> memory;

    return memory * getpagesize();
}

class measurements {
public:    
    measurements(): m_memory_usage_bytes_start(get_memory_usage_bytes()),
                    m_chrono_start(std::chrono::high_resolution_clock::now())
                    
    {
    }
    
    ~measurements() {
        const auto chrono_end = std::chrono::high_resolution_clock::now();
        const std::size_t memory_usage_bytes_end = get_memory_usage_bytes();
        
        const double nb_seconds = std::chrono::duration<double>(chrono_end - m_chrono_start).count();
        // On reads or delete the used bytes could be less than initially.
        const std::size_t used_memory_bytes = (memory_usage_bytes_end > m_memory_usage_bytes_start)?
                                                    memory_usage_bytes_end - m_memory_usage_bytes_start:0;

        std::cout << nb_seconds << "s, " << used_memory_bytes << "bytes\n";
    }
    
private:    
    std::size_t m_memory_usage_bytes_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_chrono_start;
};

void mode_test(const std::vector<std::int64_t>& keys, bool reserve = false)
{
	std::unordered_set<std::int64_t> t;
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
	    std::unordered_set<std::int64_t> t;
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
	    std::unordered_set<std::int64_t> t;
        for(std::int64_t k: keys)
        {
            t.insert(k);
        }
        std::shuffle(keys.begin(), keys.end(), generator);
        measurements m;
        for(auto key: keys)
        {
            t.find(key);
        }
    }
    else if(mode == "random-full-read-miss")
    {
        auto keys = get_random_full_ints(num_keys);
	    std::unordered_set<std::int64_t> t;
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
	    std::unordered_set<std::int64_t> t;
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
	    std::unordered_set<std::int64_t> t;
        for(std::int64_t k: keys)
        {
            t.insert(k);
        }

        measurements m;
        for(auto it = t.begin(); it != t.end(); ++it)
        {
            auto k = *it;
            (void)k;
        }
    }
	else
	{
		std::cerr << "unkown mode: " << mode << std::endl;
		return -1;
	}
	return 0;
}

#include "test_lookup_table_utils.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <unistd.h> // getpagesize
#include <unordered_set>
#include <string>

static std::random_device SEED{};
thread_local std::mt19937_64 generator(SEED());

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
                                               std::int64_t min, 
                                               std::int64_t max) 
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

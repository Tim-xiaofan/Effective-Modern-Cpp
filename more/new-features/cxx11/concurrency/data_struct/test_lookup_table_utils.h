#ifndef _TEST_LOOCKUP_TABLE_UTILS_H_
#define _TEST_LOOCKUP_TABLE_UTILS_H_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <unistd.h> // getpagesize
#include <unordered_set>
#include <string>

extern thread_local std::mt19937_64 generator;

/**
 * Generate a vector [0, nb_ints) and shuffle it
 */
std::vector<std::int64_t> get_random_shuffle_range_ints(std::size_t nb_ints);
/**
 * Generate random vector of random ints between min and max.
 */
std::vector<std::int64_t> get_random_full_ints(std::size_t nb_ints, 
                                               std::int64_t min = 0, 
                                               std::int64_t max = std::numeric_limits<std::int64_t>::max()); 

std::size_t get_memory_usage_bytes(); 

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

#endif

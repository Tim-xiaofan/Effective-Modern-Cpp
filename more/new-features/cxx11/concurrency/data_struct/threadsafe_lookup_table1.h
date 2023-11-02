#ifndef _THREADSAFE_LOOKUP_TABLE1_H_
#define _THREADSAFE_LOOKUP_TABLE1_H_

#include <functional>
#include <vector>
#include <list>
#include <utility>
#include <mutex>
#include <algorithm>
#include <cmath>
#include <stdexcept>

struct utils
{
	static bool is_prime(int num) 
    {
        if(num <= 1) // 质数是大于1的自然数
        {
            return false;
        }

        if(num <= 3) // 2, 3 是质数
        {
            return true;
        }

        if(num % 2 == 0 || num % 3 == 0)
        {
            return false;
        }

        // check 6k-1 and 6k+1
        int root = std::ceil(std::sqrt(num));
        for(int i = 5; i <= root; i += 6)
        {
            if(num % i == 0 || num % (i + 2) == 0)
            {
                return false;
            }
        }
		
        return true;
	}

	static int get_double_prime_number(int n) {
		if (n <= 0) {
			// Invalid input, return an error code or handle it as needed.
            throw std::invalid_argument("");
		}

		// Start with the first number greater than or equal to 2*n
		int num = 2 * n + 1; // skip even

		// Find the next prime number
		while (true) {
			if (is_prime(num)) {
				return num;
			}
			num += 2; // Check only odd numbers to optimize
		}
	} 
};

// add rehash mechanism
template<typename Value, typename Hash=std::hash<Value>>
class threadsafe_lookup_table1
{
    public:
        class bucket
        {// 存储具有相同 hash 的 values
            public:
                using value_type = Value;
                using value_list = std::list<value_type>;
                using iterator = typename value_list::iterator;

                bool find(const value_type& value)
                {
                    return std::find(values_.begin(), values_.end(), value)
                        != values_.end();
                }

                bool insert(const value_type& value)
                {
                    auto it = std::find(values_.begin(), values_.end(), value);
                    if(it == values_.end())
                    {
                        values_.push_back(value);
                        return true;
                    }
                    return false;
                }

                bool erase(const value_type& value)
                {
                    auto it = std::find(values_.begin(), values_.end(), value);
                    if(it != values_.end())
                    {
                        values_.erase(it);
                        return true;
                    }
                    return false;
                }

                bool empty(void) const
                {
                    return values_.empty();
                }
                 
                bool size(void) const
                {
                    return values_.size();
                }

                iterator begin()
                {
                    return values_.begin();
                }

                iterator end()
                {
                    return values_.end();
                }

            private:
                friend class threadsafe_lookup_table1;
                value_list values_;
        };

        threadsafe_lookup_table1(int num_buckets=1, Hash hash = Hash())
            :buckets_(num_buckets),
            hash_(hash),
            size_(0),
            max_load_factor_(1)
        {
        }

        bool find(const Value& value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return get_bucket(value).find(value);
        }

        void insert(const Value& value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(get_bucket(value).insert(value))
            {
                ++size_;
                if(size_ > get_max_load_factor() * get_bucket_count())
                {
                    rehash();
                }
            }
        }

        void erase(const Value& value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(get_bucket(value).erase(value))
            {
                --size_;
            }
        }

        bool empty(void) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return size_ == 0;
        }

        int size(void) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return size_;
        }

        int bucket_count(void) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return get_bucket_count();
        }

        /** @return Average number of elements per bucket*/
        double load_factor(void) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return size_ * 1.0 / get_bucket_count();
        }

        void dump(void) const
        {
            int ct = 0;
            std::lock_guard<std::mutex> lock(mutex_);
            for(const bucket& b: buckets_)
            {
                std::cout << "bucket[" << ct++ << "]:";
                for(const Value& v: b.values_)
                {
                    std::cout << v << " ";
                }
                std::cout << std::endl;
            }
        }

        void reserve(int count)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(count > get_bucket_count())
            {
                int new_bucket_ct = utils::get_double_prime_number(count);

                do_rehash(new_bucket_ct);
            }
        }

    private:

        bucket& get_bucket(const Value& value)
        {
            bucket& b = buckets_[hash_(value) % get_bucket_count()];
            return b;
        }

        void do_rehash(int new_bucket_ct)
        {
            if(new_bucket_ct > get_bucket_count())
            {
                std::vector<bucket> new_buckets(new_bucket_ct);
                for(bucket& b: buckets_)
                {
                    for(Value& value: b)
                    {
                        size_t new_hash_value = hash_(value) % new_bucket_ct;
                        new_buckets[new_hash_value].values_.emplace_back(std::move(value));
                    }
                }
                buckets_ = std::move(new_buckets);
            }
        }

        void rehash(void)
        {
            int new_bucket_ct = utils::get_double_prime_number(get_bucket_count());

            do_rehash(new_bucket_ct);
        }

        int get_bucket_count(void) const
        {
            return buckets_.size();
        }

        int get_max_load_factor(void) const
        {
            return max_load_factor_;
        }

        std::vector<bucket> buckets_;
        Hash hash_;
        mutable std::mutex mutex_;
        int size_;
        int max_load_factor_;
};

#endif

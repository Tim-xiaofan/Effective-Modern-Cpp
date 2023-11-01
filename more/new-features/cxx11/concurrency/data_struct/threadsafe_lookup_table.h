#ifndef _THREADSAFE_LOOKUP_TABLE_H_
#define _THREADSAFE_LOOKUP_TABLE_H_

#include <functional>
#include <vector>
#include <list>
#include <utility>
#include <mutex>
#include <algorithm>

template<typename Value, typename Hash=std::hash<Value>>
class threadsafe_lookup_table
{
    public:
        class bucket
        {// 存储有相同 hash 的 values
            public:
                using value_type = Value;
                using value_list = std::list<value_type>;
                using iterator = typename value_list::iterator;

                bool find(const value_type& value)
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    return std::find(values_.begin(), values_.end(), value)
                        != values_.end();
                }

                void insert(const value_type& value)
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    auto it = std::find(values_.begin(), values_.end(), value);
                    if(it == values_.end())
                    {
                        values_.push_back(value);
                    }
                }

                void erase(const value_type& value)
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    auto it = std::find(values_.begin(), values_.end(), value);
                    if(it != values_.end())
                    {
                        values_.erase(it);
                    }
                }

                bool empty(void) const
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    return values_.empty();
                }
                 
                bool size(void) const
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    return values_.size();
                }

            private:
                friend class threadsafe_lookup_table;
                value_list values_;
                mutable std::mutex mutex_;
        };

        // threadsafe_lookup_table 没有 rehash 功能，需要预先设置合适数量的 hash 桶来提升性能
        threadsafe_lookup_table(int num_buckets, Hash hash = Hash())
            :buckets_(num_buckets), hash_(hash)
        {
        }

        bool find(const Value& value)
        {
            return get_bucket(value).find(value);
        }

        void insert(const Value& value)
        {
            get_bucket(value).insert(value);
        }

        void erase(const Value& value)
        {
            get_bucket(value).erase(value);
        }

        bool empty(void) const
        {
            for(const bucket& b: buckets_)
            {
                if(!b.empty())
                {
                    return false;
                }
            }
            return true;
        }

        int size(void) const
        {
            int sz = 0;
            for(const bucket& b: buckets_)
            {
                sz += b.size();
            }
            return sz;
        }

        void reserve(int sz)
        {
        }

    private:

        bucket& get_bucket(const Value& value)
        {
            bucket& b = buckets_[hash_(value) % buckets_.size()];
            return b;
        }

        std::vector<bucket> buckets_;
        Hash hash_;
};

#endif

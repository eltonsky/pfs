#ifndef BlockQueue_H
#define BlockQueue_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <thread>
#include "Log.h"

using namespace std;

template <typename T> class BlockQueue
{
    unsigned max_size = 0;
    const int prod_wait_time = 30;
    const int cons_wait_time = 30;

    // use a dequeue for insert at both ends
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_is_empty;
    std::condition_variable cond_is_full;

    typedef std::unique_lock<std::mutex> unilock;

public:
    BlockQueue<T>(){}

    BlockQueue<T>(int capacity) {
        max_size = capacity;
    }

    virtual ~BlockQueue<T>() {

    }

    const int size() {
        unilock l(mutex_);
        return queue_.size();
    }

    const bool try_push(T const &val, bool back = true)
    {
        unilock l(mutex_);

        Log::write(DEBUG, "try_push: queue size %d, max_size %d\n",queue_.size(),max_size);

        if(cond_is_full.wait_for(l, chrono::milliseconds(prod_wait_time),
                [this] { return queue_.size() < max_size; })) {

            bool wake = queue_.empty(); // we may need to wake consumer

            if(back)
                queue_.push_back(val);
            else
                queue_.push_front(val);

            if (wake) cond_is_empty.notify_one();

            Log::write(DEBUG, "pushed value, queue size %d\n", queue_.size());

            return true;
        }

        return false;
    }


    void push(T const &val) {
        while(!try_push(val)){
            this_thread::sleep_for(chrono::milliseconds(prod_wait_time));
        }
    }


    void push_front(T const &val) {
        while(!try_push(val, false)){
            this_thread::sleep_for(chrono::milliseconds(prod_wait_time));
        }
    }


    const bool _pop(T& result)
    {
        unilock l(mutex_);

//        Log::write(DEBUG, "_pop: queue size %d, max_size %d\n", queue_.size(), max_size);

        if(cond_is_empty.wait_for(l, chrono::milliseconds(cons_wait_time),
                [this] { return queue_.size() > 0; })) {

            result = queue_.front();
            queue_.pop_front();

            if(queue_.size() == max_size - 1)
                cond_is_full.notify_one();

            return true;
        }

        return false;
    }


    const bool try_pop(T& result) {
        return _pop(result);
    }


    T pop() {
        unilock l(mutex_);

        T ret;

        while(true) {
            if(cond_is_empty.wait_for(l, chrono::milliseconds(cons_wait_time),
                    [this] { return queue_.size() > 0; })) {

                ret = queue_.front();
                queue_.pop_front();

                if(queue_.size() == max_size - 1)
                    cond_is_full.notify_one();

                break;
            }

        }

        return ret;
    }


};


#endif


#ifndef SIMPLESTOCKEXCHANGE_THREADSAFEQ_HPP
#define SIMPLESTOCKEXCHANGE_THREADSAFEQ_HPP

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <queue>
#include <iostream>
#include <boost/lockfree/queue.hpp>


namespace s21 {
    template<typename T>
    class ThreadSafeQ  : boost::noncopyable {
        using scoped_lock = boost::recursive_mutex::scoped_lock;
    public:
        ThreadSafeQ() = default;
        ThreadSafeQ(ThreadSafeQ&&) = default;
        ThreadSafeQ &operator=(ThreadSafeQ&&) = default;
        ~ThreadSafeQ() {
            Clear();
        }

        const T& Front(){
            scoped_lock lock(q_mutex_);
            return message_que_.front();
        }

        const T& Back(){
            scoped_lock lock(q_mutex_);
            return message_que_.back();
        }

        T PopFront(){
            scoped_lock lock(q_mutex_);
            auto t = std::move(message_que_.front());
            message_que_.pop_front();
            return t;
        }

        T PopBack(){
            scoped_lock lock(q_mutex_);
            auto t = std::move(message_que_.back());
            message_que_.pop_back();
            return t;
        }


        template<typename ... Args>
        void EmplaceBack(Args&& ... args){
            std::cout << "starting to emplace\n";
            scoped_lock lock(q_mutex_);
            std::cout << "locked\n";
            message_que_.emplace_back(std::forward<Args>(args)...);
            std::cout << "data is in \n";

//            boost::unique_lock<boost::mutex> unique_lock(blocking_mutex_);
//            cv_block_.notify_one();
        }


        template<typename ... Args>
        void EmplaceFront(Args&& ... args){
            scoped_lock lock(q_mutex_);
            message_que_.emplace_front(std::forward<Args>(args)...);

//            boost::unique_lock<boost::mutex> unique_lock(blocking_mutex_);
//            cv_block_.notify_one();
        }

        void Erase(T& value){
            scoped_lock lock(q_mutex_);
            message_que_.erase(std::remove(message_que_.begin(), message_que_.end(), value), message_que_.end());
        }

        bool Empty(){
            scoped_lock lock(q_mutex_);
            return message_que_.empty();
        }

        void Clear(){
            scoped_lock lock(q_mutex_);
            message_que_.clear();
        }

//        void Wait(){
//            while(Empty()){
//                boost::unique_lock<boost::mutex> unique_lock(blocking_mutex_);
//                cv_block_.wait(unique_lock);
//            }
//        }

    private:
        std::deque<T> message_que_;
//        boost::condition_variable cv_block_;
        boost::recursive_mutex q_mutex_;
//        boost::mutex blocking_mutex_;

    };
}

#endif //SIMPLESTOCKEXCHANGE_THREADSAFEQ_HPP

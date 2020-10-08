#include "strm_queue.hpp"
#include <iostream>

void strm_queue::push(char* data)
{
    this->mtx.lock();
    this->queue.push(std::string(data));
    this->mtx.unlock();
    this->notify();
}

void strm_queue::push(std::string data)
{
	// DEBUG: std::cout << "queue size: " << this->queue.size() << std::endl;
    this->mtx.lock();
    this->queue.push(data);
    this->mtx.unlock();
    this->notify();
}

std::string strm_queue::pop()
{
    this->mtx.lock();
    std::string t = this->queue.front();
    this->queue.pop();
    this->mtx.unlock();
    return t;
}

std::string strm_queue::front()
{
    this->mtx.lock();
    std::string t = this->queue.front();
    this->mtx.unlock();
    return t;
}

std::size_t strm_queue::size()
{
    size_t t;
    this->mtx.lock();
    t = this->queue.size();
    this->mtx.unlock();
    return t;
}

void strm_queue::lock()
{
    this->mtx.lock();
}

void strm_queue::unlock()
{
    this->mtx.unlock();
}

void strm_queue::wait_if_empty()
{
    std::unique_lock <std::mutex> lck(this->mtx);
    while(this->queue.size() == 0)
        this->cv.wait(lck);
}

void strm_queue::notify()
{
    cv.notify_all();
}

bool strm_queue::empty(){
	
	if(this->queue.size() == 0){
		return true;
	}else{
		return false;
	}
	
}
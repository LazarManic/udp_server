#ifndef _STRM_QUEUE_H_
#define _STRM_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

/*
 * Class used to create a thread safe queue
 * with waiting and notifying capabilities
 */
class strm_queue
{
private:
    std::queue<std::string> queue;
    std::mutex mtx;
    std::condition_variable cv;

    void notify(); // Notifies the thread blocked in wait_if_empty to unblock

public:
	/* Add new element to end of queue */
    void push(char* data); 
    void push(std::string data);
	
	/* Pops the first element of the queue and returns it */
    std::string pop(); 
	
	/* Returns the first element of the queue */
    std::string front(); 
	/* Returns the queue size */
    std::size_t size(); 
	/* Locks the queue. Other threads must wait for unlocking */
    void lock(); 
	/* Unlocks the queue */
    void unlock(); 
	/* Thread blocks here if queue is empty. Unblock when something is 
	 * pushed to queue */
    void wait_if_empty();  
	/* Checks if queue is empty*/
	bool empty();
	
                            
};

#endif

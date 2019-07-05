#ifndef _GLOBAL_TYPES_HPP
#define _GLOBAL_TYPES_HPP

#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include "protocol.hpp"

#define MAX_LEN 1000

struct data_t
{
	char raw[MAX_LEN];
	unsigned len;
	std::string formatted;
};

struct msg_t
{
	std::string type;
    std::string cmd_id;
    std::string content;
    std::string sensor_name;
    double timestamp;
    std::vector<double> plot_data;
};

struct board_command_t
{
	char* command;
	char* value;
	board_command_t* next;
};

struct board_command_list_t
{
	board_command_t* current;
	board_command_t* next;
};

struct board_msg_t
{
	char* type;
	char* cmd_id;
	char* content;
	char* sensor_name;
	double timestamp;
	board_command_t* head;
};

enum udp_states
{
	init_state,
	board_info_state,
	receive_state,
	parse_exe_state,
	send_state,
	wait_state,
	idle_state,
	stopped_state
};

class messages
{
public:

	messages(unsigned s_id) : shuttle_id(s_id), 
		connection_cmd(std::string(MSG_COMMAND)+DELIMITER+CMD_CONNECT+DELIMITER),
		connection_rsp(std::string(MSG_RESPONSE)+DELIMITER+CMD_CONNECT+DELIMITER),
		disconnection_cmd(std::string(MSG_COMMAND)+DELIMITER+CMD_DISCONNECT+DELIMITER),
		boardinfo_cmd(std::string(MSG_COMMAND)+DELIMITER+CMD_BOARD_INFO+DELIMITER+std::to_string(s_id)),
		boardinfo_rsp(std::string(MSG_RESPONSE)+DELIMITER+CMD_BOARD_INFO+DELIMITER+RSP_OK)
	{

	}

	unsigned shuttle_id;

	const std::string connection_cmd;
    const std::string connection_rsp;
	const std::string disconnection_cmd;
    const std::string boardinfo_cmd;
    const std::string boardinfo_rsp;
};

class wait_semaphore
{
public:
	wait_semaphore(unsigned l)
	{
		locks = l;
	}
	bool acquire()
	{
		bool ret_val = false;
		std::unique_lock<std::mutex> lock(mtx);
		var_mtx.lock();
		if(locks>1)
		{
			locks--;
			ret_val = true;
			var_mtx.unlock();
			cv.wait(lock);
		}
		else
		{
			var_mtx.unlock();
			cv.notify_all();
		}

		return ret_val;
	}

private:
	unsigned locks;
	std::mutex mtx;
	std::condition_variable cv;
	std::mutex var_mtx;

	bool get_locks()
	{
		return this->locks;
	}
};

class atomic_bool
{
public:
	atomic_bool() : val(false){}

	void set_true()
	{
		this->val_mtx.lock();
		this->val = true;
		this->val_mtx.unlock();
		this->cv.notify_all();
	}

	void set_false()
	{
		this->val_mtx.lock();
		this->val = false;
		this->val_mtx.unlock();
		this->cv.notify_all();
	}

	bool get_val()
	{
		bool temp;
		this->val_mtx.lock();
		temp = this->val;
		this->val_mtx.unlock();
		return temp;
	}

	void wait_for_true()
	{
		std::unique_lock<std::mutex> lock(mtx);
		while(!this->val)
			this->cv.wait(lock);
	}

	void wait_for_false()
	{
		std::unique_lock<std::mutex> lock(mtx);
		while(this->val)
			this->cv.wait(lock);
	}

private:
	std::mutex mtx;
	std::condition_variable cv;
	std::mutex val_mtx;
	bool val;
};

#endif

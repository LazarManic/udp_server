#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <string>
#include <vector>
#include <cmath>

#include "asio.hpp"
#include "udp_if.hpp"
#include "strm_queue.hpp"
#include "protocol.hpp"

extern "C" void init_server(unsigned int, unsigned int, unsigned int);
extern "C" void init_server_blocking(unsigned int, unsigned int, unsigned int);

extern "C" void set_board_handler(void (*f)(board_msg_t*));


extern "C" void send_data_s3(char* sensor_name, float timestamp,
						float x, float y, float z);
extern "C" void send_data_s1(char* sensor_name, float timestamp, float x);
extern "C" void send_data_sb(char* sensor_name, float timestamp,
							char* signal_name, unsigned int x);

static udp_if* udp_if_ptr;

void init_server(unsigned int command_port, unsigned int stream_port, unsigned int shuttleBoardID)
{
	std::string cmd_port = std::to_string(command_port);
    std::string strm_port = std::to_string(stream_port);
	::udp_if_ptr = new udp_if(cmd_port, strm_port, shuttleBoardID);
	::udp_if_ptr->start_state_machine();
}

void init_server_blocking(unsigned int command_port, unsigned int stream_port, unsigned int shuttleBoardID)
{
	std::string cmd_port = std::to_string(command_port);
    std::string strm_port = std::to_string(stream_port);
	::udp_if_ptr = new udp_if(cmd_port, strm_port, shuttleBoardID);
	::udp_if_ptr->start_state_machine_blocking();
}

void set_board_handler(void (*f)(board_msg_t*))
{
	udp_if_ptr->setBoardHandler(f);
}

void send_data_s3(char* sensor_name, float timestamp, float x, float y, float z)
{
	std::string message = std::string(MSG_SENSOR_3_AXIS)+DELIMITER+
								std::string(sensor_name)+DELIMITER+
								std::to_string(timestamp)+DELIMITER+
								std::to_string(x)+AXIS_DELIMITER+
								std::to_string(y)+AXIS_DELIMITER+
								std::to_string(z);

	::udp_if_ptr->push(message);
}

void send_data_s1(char* sensor_name, float timestamp, float x)
{
	std::string message = std::string(MSG_SENSOR_1_AXIS)+DELIMITER+
								std::string(sensor_name)+DELIMITER+
								std::to_string(timestamp)+DELIMITER+
								std::to_string(x);

	::udp_if_ptr->push(message);
}


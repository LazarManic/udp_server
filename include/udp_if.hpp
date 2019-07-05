#ifndef _UDP_IF_HPP_
#define _UDP_IF_HPP_

#include <string>
#include "udp_channel.hpp"
#include "udp_parser.hpp"
#include "strm_queue.hpp"
#include "board_if.hpp"

class udp_if
{
public:
	/* Constructor */
	udp_if(std::string cmd_p, std::string strm_p, unsigned s_id);
	/* Creates 2 new threads and starts the cmd and strm state machines */
	void start_state_machine();

	void start_state_machine_blocking();

	void setBoardHandler(void (*f)(board_msg_t*));

    void push(char* data); 
    void push(std::string data);
	/* Queue with support for multithreaded use */
	strm_queue strm_data;

private:
	/* Objects used to send data through the UDP channels */
	udp_channel cmd_channel;
	udp_channel strm_channel;

	/* Board shuttle ID */
	unsigned shuttle_id;

	/* Objects for multithreading and synchronization */
	std::thread* udp_cmd_worker;
	std::thread* udp_strm_worker;
	atomic_bool strm_enabled;
	atomic_bool server_inited;

	/* State machines for command channel and stream channel */
	void cmd_state_machine();
	void strm_state_machine();

	/* Collection of preformatted messages for connecting, sending board info */
	messages msgs;

	/* Object used for parsing received messages and responding */
	udp_parser* parser;
	board_if* board;

	/* Method used for executing received command or sending response */
	void execute();
};

#endif

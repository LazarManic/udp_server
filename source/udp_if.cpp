#include <iostream>
#include <thread>
#include "udp_if.hpp"
#include "global_types.hpp"
#include "protocol.hpp"


udp_if::udp_if(std::string cmd_p, std::string strm_p, unsigned s_id) : 
	cmd_channel(cmd_p), strm_channel(strm_p), shuttle_id(s_id),
	init_semaphore(2), msgs(s_id)
{
	this->parser = new udp_parser;
	this->board = new board_if(s_id);
}

void udp_if::start_state_machine()
{
	this->udp_cmd_worker = new std::thread(&udp_if::cmd_state_machine, this);
	this->udp_strm_worker = new std::thread(&udp_if::strm_state_machine, this);
}

void udp_if::start_state_machine_blocking()
{
	this->start_state_machine();
	server_inited.wait_for_true();
}

void udp_if::setBoardHandler(void (*f)(board_msg_t*))
{
	this->board->handler = f;
}

void udp_if::cmd_state_machine()
{
	while(true)
	{
		/* Get raw data from the UDP channel - init cmd_channel.data struct */
		switch(this->cmd_channel.state)
		{
			case init_state:
				this->cmd_channel.receive_data();
				if(this->cmd_channel.data.formatted == msgs.connection_cmd)
				{
					/* Send connection response - connection established */
					this->cmd_channel.send_data(msgs.connection_rsp+std::to_string(this->board->get_shuttle_id()));
					this->cmd_channel.state = wait_state;
				}
				else
				{
					std::cerr 	<< "ERROR: UDP CMD server did not receive valid"
								<< " connection message" << std::endl;
				}
				break;
	
			case board_info_state:
				this->cmd_channel.send_data(msgs.boardinfo_cmd);	
				this->cmd_channel.receive_data();
				if(this->cmd_channel.data.formatted == msgs.boardinfo_rsp)
				{
					if(server_inited.get_val())
					{	
						this->cmd_channel.state = receive_state;
						std::cout << "Client reconnected" << std::endl;
					}

					else
						this->cmd_channel.state = wait_state;
				}
				else
				{
					this->cmd_channel.state = init_state;
					std::cerr 	<< "ERROR: Wrong board configured" << std::endl;
				}
				break;	

			case wait_state:
				this->init_semaphore.acquire();
				this->cmd_channel.state = receive_state;
				this->server_inited.set_true();
				break;
	
			case receive_state:
				this->cmd_channel.receive_data();
				this->cmd_channel.state = parse_exe_state;	
				break;	
	
			case parse_exe_state:
				if(!this->parser->parse(this->cmd_channel.data.formatted))
					std::cerr << "ERROR: Invalid command received" << std::endl;
				else
					this->execute();
				this->cmd_channel.state = receive_state;
				break;
	
			default:
				std::cerr 	<< "ERROR: UDP CMD server has reached an invalid "
							<< "state" << std::endl;
				break;	
		}
	}
}

void udp_if::strm_state_machine()
{
	while(true)
	{
		switch(this->strm_channel.state)
		{
			case init_state:
				this->strm_channel.receive_data();
				if(this->strm_channel.data.formatted == msgs.connection_cmd)
				{
					/* Send connection response - connection established */
					this->strm_channel.send_data(msgs.connection_rsp+std::to_string(this->board->get_shuttle_id()));
					this->strm_channel.state = wait_state;
				}

				break;

			case wait_state:
				this->init_semaphore.acquire();
				this->strm_channel.state = idle_state;
				this->server_inited.set_true();
				break;

			case send_state:
				this->strm_channel.send_data(this->strm_data.pop());	
				this->strm_channel.state = idle_state;
				break;
			
			case idle_state:
				this->strm_data.wait_if_empty();
				this->strm_channel.state = send_state;
				
				if(!this->strm_enabled.get_val())
					this->strm_channel.state = stopped_state;
				break;

			case stopped_state:
				this->strm_enabled.wait_for_true();
				this->strm_channel.state = idle_state;
				break;

			default:
				std::cerr << "UDP STRM server has reached an invalid state" << std::endl;
				break;
		}
	}
}

void udp_if::execute()
{
    /* List of things to do for every command */
	/* FOR DEBUG */
	/*
	std::cout << this->parser->message.type << std::endl;
	std::cout << this->parser->message.cmd_id << std::endl;
	std::cout << this->parser->message.content << std::endl;
	*/

    /* If message is a command */
    if(this->parser->message.type == MSG_COMMAND)
    {
        if(this->parser->message.cmd_id == CMD_G_STATUS)
        {
			/* TODO: replace + 1 with the strm channel ready bit */
            std::string ts = std::string(MSG_RESPONSE)+DELIMITER+CMD_G_STATUS+DELIMITER+"1";

			this->cmd_channel.send_data(ts);
        }
        else if(this->parser->message.cmd_id == CMD_SM_PACK)
        {
        }
        else if(this->parser->message.cmd_id == CMD_STREAM_START)
        {
			this->strm_enabled.set_true();
        }
        else if(this->parser->message.cmd_id == CMD_STREAM_STOP)
        {
			this->strm_enabled.set_false();
        }
		else if(this->parser->message.cmd_id == CMD_DISCONNECT)
		{
			this->cmd_channel.state = init_state;
			this->strm_enabled.set_false();
			std::cout << "Client disconnected. UDP server is still running." << std::endl;
		}
		else if(this->parser->message.cmd_id == CMD_CONNECT)
		{
			this->cmd_channel.send_data(msgs.connection_rsp);
		}
		else
		{
			std::cerr << "UDP server has received an invalid command" << std::endl;
		}
    }

    /* If message is a response */
    else if(this->parser->message.type == MSG_RESPONSE)
    {
        // do whatever
    }

	/* If message is a 3 axis sensor data stream */
    else if(this->parser->message.type == MSG_SENSOR_3_AXIS)
    {
        // put sensor data in plotting queue
    }

    /* If message is a 1 axis sensor data stream */
    else if(this->parser->message.type == MSG_SENSOR_1_AXIS)
    {
        // put sensor data in plotting queue
    }

	else if(this->parser->message.type == MSG_BOARD)
	{
		this->board->execute(&this->parser->message);
	}


}

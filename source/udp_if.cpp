#include <iostream>
#include <thread>
#include "udp_if.hpp"
#include "global_types.hpp"
#include "protocol.hpp"


udp_if::udp_if(std::string cmd_p, std::string strm_p, unsigned s_id) : 
	cmd_channel(cmd_p), strm_channel(strm_p), shuttle_id(s_id),
	msgs(s_id),
	bytes_readable_cmd(true), bytes_readable_strm(true)
{
	/* IO control for getting bytes written */
	this->cmd_channel.socket.io_control(this->bytes_readable_cmd);
	this->strm_channel.socket.io_control(this->bytes_readable_strm);
	
	this->parser = new udp_parser;
	this->board = new board_if(s_id);
}

void udp_if::start_state_machine()
{
	std::cout << "Created threads" << std::endl;
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

void udp_if::push(char* data)
{
	if(this->strm_enabled.get_val())
		this->strm_data.push(data);
}

void udp_if::push(std::string data)
{
	if(this->strm_enabled.get_val())
		this->strm_data.push(data);
}

void udp_if::cmd_state_machine()
{
	while(true)
	{
		/* Check if server is inited and update cmd_channel state */
		if(this->cmd_inited.get_val() == false){
			this->cmd_channel.state = init_state;
		}
		/* Get raw data from the UDP channel - init cmd_channel.data struct */
		switch(this->cmd_channel.state)
		{
			case init_state:
				this->cmd_channel.receive_data();
				if(this->cmd_channel.data.formatted == msgs.connection_cmd)
				{
					/* Send connection response - connection established */
					std::cout << "Cmd socket connected" << std::endl;
					this->cmd_inited.set_true();
					this->cmd_channel.send_data(msgs.connection_rsp+std::to_string(this->board->get_shuttle_id()));
					this->cmd_channel.state = wait_state;
										
				}
				else
				{
					std::cerr 	<< "ERROR: UDP CMD server did not receive valid"
								<< " connection message:'" << this->cmd_channel.data.formatted << "'" <<std::endl ;
				}
				break;
	
			case wait_state:
				
				
				while(!this->strm_inited.get_val()){
					this->cmd_channel.receive_data();
					
				}
				
				if(this->cmd_inited.get_val() == true){
					this->server_inited.set_true();
					this->cmd_channel.state = receive_state;
				}else{
					this->cmd_channel.state = init_state;
				}

				break;
	
			case receive_state:
				this->cmd_channel.receive_data();
				this->cmd_channel.state = parse_exe_state;	
				break;	
	
			case parse_exe_state:
				this->cmd_channel.state = receive_state;
				if(!this->parser->parse(this->cmd_channel.data.formatted))
					std::cerr << "ERROR: Invalid command received" << std::endl;
				else
					this->execute();
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
		/* Check if server is inited and update strm_channel state */
		if(this->strm_inited.get_val() == false){
			this->strm_channel.state = init_state;
		}
		
		switch(this->strm_channel.state)
		{
			case init_state:
				this->strm_channel.receive_data();
				if(this->strm_channel.data.formatted == msgs.connection_cmd)
				{
					/* Send connection response - connection established */
					std::cout << "Stream socket connected." << std::endl;
					this->strm_channel.send_data(msgs.connection_rsp+std::to_string(this->board->get_shuttle_id()));
					this->strm_inited.set_true();
					this->strm_channel.state = wait_state;
				}
				break;

			case wait_state:
				
				this->cmd_inited.wait_for_true();
				if(this->strm_inited.get_val() == true){
					this->server_inited.set_true();
					this->strm_channel.state = idle_state;
				}else{
					this->strm_channel.state = init_state;
				}
				
				break;

			case send_state:
				
				/* empty buffer contents */
				while(this->strm_data.size())
					this->strm_channel.send_data(this->strm_data.pop());	
				this->strm_channel.state = idle_state;
				
				break;
			case receive_state:
				
				this->strm_channel.receive_data();
				if(this->strm_channel.data.formatted == msgs.disconnection_cmd)
				{
					this->strm_inited.set_false();
					this->strm_channel.state = init_state;
	
				}else{
					std::cout  << "ERROR: stream sent invalid command" << std::endl;
					this->strm_channel.state = idle_state;
				}
				
				break;
			case idle_state:
				this->strm_channel.socket.io_control(this->bytes_readable_strm);

				while(this->strm_data.empty() == true && bytes_readable_strm.get() == 0){
					this->strm_channel.socket.io_control(this->bytes_readable_strm);
				}
				if(bytes_readable_strm.get() > 0){
					this->strm_channel.state = receive_state;
				}
				else if(this->strm_enabled.get_val() == true && this->strm_data.empty() == false){
					this->strm_channel.state = send_state;
				}
				
				else if(!this->strm_enabled.get_val()){
					this->strm_channel.state = stopped_state;
				}
				break;
			
			case stopped_state:
				
				this->strm_channel.socket.io_control(this->bytes_readable_strm);
				while(!this->strm_enabled.get_val() && bytes_readable_strm.get() == 0){
					this->strm_channel.socket.io_control(this->bytes_readable_strm);
				}
				
				if(bytes_readable_strm.get() > 0){
					this->strm_channel.state = idle_state;
				}else if(this->strm_enabled.get_val()){
					this->strm_channel.state = send_state;
				}
				
				break;

			default:
				std::cerr << "UDP STRM server has reached an invalid state" << std::endl;
				strm_channel.state = init_state;
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
			std::cout << "Stream enabled" <<std::endl;
			this->strm_enabled.set_true();
        }
        else if(this->parser->message.cmd_id == CMD_STREAM_STOP)
        {
			this->strm_enabled.set_false();
        }
		else if(this->parser->message.cmd_id == CMD_DISCONNECT)
		{	
			this->cmd_channel.state = init_state;
			this->strm_channel.state = init_state;
			this->server_inited.set_false();
			this->cmd_inited.set_false();
			this->strm_inited.set_false();
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

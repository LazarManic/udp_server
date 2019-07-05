#include <iostream>
#include <map>
#include <string.h>
#include "board_if.hpp"

board_if::board_if(unsigned s_id)
{
	this->shuttle_id = s_id;
}

unsigned board_if::get_shuttle_id()
{
	return this->shuttle_id;
}

void board_if::execute(msg_t* msg)
{
	std::cout << "checkpoint -1" << std::endl;
	board_msg_t* board_msg = new board_msg_t;

	board_msg->type =  new char[msg->type.size()+1];
	std::copy(msg->type.begin(), msg->type.end(), board_msg->type);

	board_msg->cmd_id =  new char[msg->cmd_id.size()+1];
	std::copy(msg->cmd_id.begin(), msg->cmd_id.end(), board_msg->cmd_id);

	board_msg->content =  new char[msg->content.size()+1];
	std::copy(msg->content.begin(), msg->content.end(), board_msg->content);


	std::string content = msg->content;
	board_msg->head = new board_command_t;
	board_msg->head = nullptr;
	board_msg->head->next = nullptr;
	board_command_t* current = new board_command_t;
	board_msg->head = current;

	size_t pos = 0;
	while((pos = content.find(";")) != std::string::npos);
	{
		std::cout << "checkpoint 0" << std::endl;
		std::string token;
		std::string command;
		std::string value;
		token = content.substr(0, pos);
		command = token.substr(0, content.find(","));
		token.erase(0, token.find(",")+1);
		value = token;
		content.erase(0, pos+1);
		/* Local vars content and value have useful information */

		std::cout << "checkpoint 1" << std::endl;
		current->command = new char[command.length()+1];
		strcpy(board_msg->head->command, command.c_str());
		current->value = new char[value.length()+1];
		strcpy(board_msg->head->value, value.c_str());

		std::cout << "checkpoint 2" << std::endl;
		current->next = new board_command_t;
		current = current->next;
		current = nullptr;
		std::cout << "checkpoint 3" << std::endl;
	}

	this->handler(board_msg);

	delete [] board_msg->type;
	delete [] board_msg->cmd_id;
	delete [] board_msg->content;
	delete board_msg;
	/*
	if(this->shuttle_id == 70)
	{
		//bmi08x_dev bmi08xdev
		msg->cmd_id;
		msg->content;
		if(msg->cmd_id == "ACCEL_POWER_MODE")
		{
			content_map[msg->content];
		}
			
		std::cout << "executing command of board bmi085" << std::endl;
	}
	else
	{
			std::cout << "board not defined" << std::endl;
	}
	*/
}

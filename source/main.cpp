#include <iostream>
#include "asio.hpp"
#include "udp_if.hpp"
#include <string.h>

#define TYPE "B"
#define CMD_ID "BOARD_CONFIG"
#define CONTENT "ABC,DEF;GHI,JKL;MNO,PRS"

void fakeHandler(board_msg_t* board_msg)
{
	std::cout << "fake handler reached" << std::endl;
	std::cout << board_msg->type << std::endl;
	std::cout << board_msg->cmd_id << std::endl;
	std::cout << board_msg->content << std::endl;
	std::cout << strcmp(board_msg->type, TYPE) << std::endl;
	std::cout << strcmp(board_msg->cmd_id, CMD_ID) << std::endl;
	std::cout << strcmp(board_msg->content, CONTENT) << std::endl;
}


int main()
{
	std::cout << "hello world" << std::endl;

	udp_if udpIf("1234", "1235", 70);
	udpIf.setBoardHandler(fakeHandler);
	udpIf.start_state_machine();

	while(true)
	{
		udpIf.push(std::string("sample data\n"));
		std::cin.ignore();
	}

	return 0;
}

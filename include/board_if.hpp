#ifndef _BOARD_IF_H_
#define _BOARD_IF_H
#include "global_types.hpp"
#include <map>

class board_if
{
public:
	board_if(unsigned s_id);
	void execute(msg_t* msg);

	void (*handler)(board_msg_t*);

	unsigned get_shuttle_id();

private:
	unsigned shuttle_id;
	std::map<std::string, unsigned> content_map;
};


#endif

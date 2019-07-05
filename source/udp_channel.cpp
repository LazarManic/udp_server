#include "udp_channel.hpp"
#include <iostream>

udp_channel::udp_channel(std::string p) : 
	port(p), 
	socket(context, udp::endpoint(asio::ip::address::from_string("127.0.0.1"), std::stoi(p))),
	state(init_state)
{
}

void udp_channel::receive_data()
{
	this->data.len = this->socket.receive_from(
			asio::buffer(this->data.raw, MAX_LEN), 
			this->endpoint);

	/* Format raw data into string of correct length */
	this->data.formatted = std::string(this->data.raw, this->data.len);
    this->data.formatted.erase(std::remove(this->data.formatted.begin(), 
				this->data.formatted.end(), '\n'), this->data.formatted.end());

	/* For debug */
	//std::cout << "msg is: " << this->data.formatted;
}

void udp_channel::send_data(std::string msg)
{
	std::strcpy(this->data.raw, msg.c_str());
	this->socket.send_to(asio::buffer(this->data.raw, msg.length()), 
				this->endpoint);
}

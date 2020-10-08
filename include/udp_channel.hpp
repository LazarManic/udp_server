#ifndef _UDP_CHANNEL_HPP_
#define _UDP_CHANNEL_HPP_

#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include "asio.hpp"
#include "global_types.hpp"

using boost::asio::ip::udp;


class udp_channel
{
public:
	udp_channel(std::string p);

	void receive_data();
	void send_data(std::string msg);
	
	std::string port;
	boost::asio::io_context context;
	udp::endpoint endpoint;
	udp::socket socket;
	udp_states state;
	data_t data;

private:
};

#endif

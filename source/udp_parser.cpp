#include <iostream>
#include <regex>
#include "udp_parser.hpp"
#include "protocol.hpp"

std::string udp_parser::delimiter = DELIMITER;
std::string udp_parser::axis_delimiter = AXIS_DELIMITER;

void udp_parser::clear()
{
    this->message.type.clear();
    this->message.cmd_id.clear();
    this->message.content.clear();
    this->message.sensor_name.clear();
    this->message.timestamp = 0;
    while(this->message.plot_data.size() > 0)
        this->message.plot_data.pop_back();
}

udp_parser::udp_parser()
{
    this->clear();
}

std::string udp_parser::get_next_token(std::string &str)
{
    std::string t;
    std::size_t pos;
    pos = str.find(this->delimiter);

    if(pos == std::string::npos)
        pos = str.size();
    t = str.substr(0, str.find(this->delimiter));

    if(pos == str.size())
        str.erase(0, pos);
    else
        str.erase(0, pos + this->delimiter.length());

    return t;
}

std::string udp_parser::get_next_axis_point(std::string &str)
{
    std::string t;
    t = str.substr(0, str.find(this->axis_delimiter));
    str.erase(0, str.find(this->axis_delimiter) + this->axis_delimiter.length());

    return t;
}

bool udp_parser::parse(std::string raw_msg)
{
    this->clear();
    raw_msg.erase(std::remove(raw_msg.begin(), raw_msg.end(), '\n'), raw_msg.end());


    this->message.type = this->get_next_token(raw_msg);

	//std::cout << message.type;
    if(this->message.type == MSG_COMMAND)
    {
        std::regex reg_format("(.*)(:)(.*)");
        if(!(std::regex_match(raw_msg, reg_format)))
            return false;
        this->message.cmd_id = this->get_next_token(raw_msg);
        this->message.content = this->get_next_token(raw_msg);
    }
    else if(this->message.type == MSG_RESPONSE)
    {
        std::regex reg_format("(.*)(:)(.*)");
        if(!(std::regex_match(raw_msg, reg_format)))
            return false;
        this->message.cmd_id = this->get_next_token(raw_msg);
        this->message.content = this->get_next_token(raw_msg);
    }
    else if(this->message.type == MSG_SENSOR_3_AXIS)
    {
        std::regex reg_format("(.*)(:)(.*)(:)(.*)(,)(.*)(,)(.*)");
        if(!(std::regex_match(raw_msg, reg_format)))
            return false;
        this->message.sensor_name = this->get_next_token(raw_msg);
        this->message.timestamp = std::stod(this->get_next_token(raw_msg));
        this->message.plot_data.push_back(std::stod(this->get_next_axis_point(raw_msg)));
        this->message.plot_data.push_back(std::stod(this->get_next_axis_point(raw_msg)));
        this->message.plot_data.push_back(std::stod(this->get_next_axis_point(raw_msg)));
    }
    else if(this->message.type == MSG_SENSOR_1_AXIS)
    {
        std::regex reg_format("(.*)(:)(.*)(:)(.*)");
        if(!(std::regex_match(raw_msg, reg_format)))
            return false;
        this->message.sensor_name = this->get_next_token(raw_msg);
        this->message.timestamp = std::stod(this->get_next_token(raw_msg));
        this->message.plot_data.push_back(std::stod(this->get_next_axis_point(raw_msg)));
    }
	else if(this->message.type == MSG_BOARD)
    {
        std::regex reg_format("(.*)(:)(.*)");
		std::cout << "board msg detected" ;
        if(!(std::regex_match(raw_msg, reg_format)))
            return false;
        this->message.cmd_id = this->get_next_token(raw_msg);
        this->message.content = this->get_next_token(raw_msg);
    }
    else
    {
        return false;
    }

    if(raw_msg.empty())
        return true;
    else return false;
}

std::string udp_parser::generate_response()
{
    // response generated with help from board_if if needed
    return std::string();
}

void udp_parser::test()
{
    std::cout << "type: " << this->message.type << std::endl;
    std::cout << "cmd_id: " << this->message.cmd_id << std::endl;
    std::cout << "content: " << this->message.content << std::endl;
    std::cout << "sensor_name: " << this->message.sensor_name << std::endl;
    std::cout << "timestamp: " << this->message.timestamp << std::endl;
    while(this->message.plot_data.size() > 0)
    {
        std::cout << "plot data: " << this->message.plot_data.back() << std::endl;
        this->message.plot_data.pop_back();
    }
}



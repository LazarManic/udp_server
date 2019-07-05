#ifndef _UDP_PARSER_H_
#define _UDP_PARSER_H_

#include <string>
#include <vector>
#include "udp_channel.hpp"

/*
 * udp_parser is a base class that is used to parse the received message into
 * a defined msg_t struct. It can also execute the command and generate a resp.
*/
class udp_parser
{
private:
    /* Delimiters - default is : and , for axis delimiter */
    static std::string delimiter;
    static std::string axis_delimiter;

    /* Clears the message contents from previous use, or upon object creation */
    void clear();
    /* Process raw received string to seperate type, cmd_id, contents, etc. */
    std::string get_next_token(std::string &str);
    /* Process raw received string to seperate plot data */
    std::string get_next_axis_point(std::string &str);

public:
    /* Constructor */
    udp_parser();
    /* Parse raw message into the struct msg_t message member */
    bool parse(std::string raw_msg);
    /* generate response - obsolete? */
    std::string generate_response();
    /* Print out parsed message to std out */
    void test();

    /* Parsed message */
    msg_t message;
};


#endif

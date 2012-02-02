#include "stdafx.h"
#include "command.cpp"

using std::string;
using std::vector;
using boost::lexical_cast;
using boost::algorithm::join;
using boost::algorithm::split;

#ifndef PROTOCOL_CPP
#define PROTOCOL_CPP

#define PROTOCOL_HEADER "cgi4lcd"
#define PROTOCOL_DELIMITER "|"

class protocol {

public:

    static command parse(string data) {

        vector<string> packet;
        command cmd;

        split(packet, data, boost::is_any_of(PROTOCOL_DELIMITER));

        if (packet.size() != 6) {
            cmd.is_malformed = true;
        }
        else {
            if (packet[0] != PROTOCOL_HEADER) {
                cmd.is_malformed = true;
            }
            else if (packet[1] == "command" && packet[2] != "")
                cmd.is_internal = true;
                cmd.executable = packet[1];
                cmd.arguments = packet[2];
            else {
                cmd.executable = packet[1];
                cmd.arguments = packet[2];
                cmd.interval = lexical_cast<unsigned int>(packet[3]);
                cmd.timeout = lexical_cast<unsigned int>(packet[4]);
                cmd.is_malformed = false;
            }
        }

        return cmd;
        
    }

    static string build(const command &cmd) { 

        vector<string> packet;

        packet.push_back(PROTOCOL_HEADER);
        packet.push_back(cmd.executable);
        packet.push_back(cmd.arguments);
        packet.push_back(lexical_cast<string>(cmd.interval));
        packet.push_back(lexical_cast<string>(cmd.timeout));
        packet.push_back("");

        return join(packet, PROTOCOL_DELIMITER);
        
    }

};

#endif // PROTOCOL_CPP


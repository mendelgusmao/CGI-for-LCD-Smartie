#include "stdafx.h"
#include "Protocol.h"

using boost::lexical_cast;
using boost::algorithm::join;
using boost::algorithm::split;

Command Protocol::parse(const string &data) {

    vector<string> packet;
    Command command;

    split(packet, data, boost::is_any_of(PROTOCOL_DELIMITER));

    if (packet.size() != PROTOCOL_EXPECTED_SIZE) {
        command.is_malformed = true;
    }
    else {
        if (packet[0] != PROTOCOL_HEADER) {
            command.is_malformed = true;
        }
        else if (packet[1] == "command" && packet[2] != "") {
            command.is_internal = true;
            command.executable = packet[2];
            command.arguments = packet[3];
        }
        else {
            command.executable = packet[1];
            command.arguments = packet[2];
            command.interval = lexical_cast<unsigned int>(packet[3]);
            command.timeout = lexical_cast<unsigned int>(packet[4]);
            command.do_not_queue = packet[5] == "1";
            command.add_and_run = packet[6] == "1";
            command.is_malformed = false;
        }
    }

    return command;
    
}

string Protocol::build(const Command &command) { 

    vector<string> packet;

    packet.push_back(PROTOCOL_HEADER);
    packet.push_back(command.executable);
    packet.push_back(command.arguments);
    packet.push_back(lexical_cast<string>(command.interval));
    packet.push_back(lexical_cast<string>(command.timeout));
    packet.push_back(command.do_not_queue ? "1" : "0");
    packet.push_back(command.add_and_run ? "1" : "0");
    packet.push_back("");

    return join(packet, PROTOCOL_DELIMITER);
    
}

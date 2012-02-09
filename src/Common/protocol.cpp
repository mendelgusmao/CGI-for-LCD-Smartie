#include "protocol.h"

using boost::lexical_cast;
using boost::algorithm::join;
using boost::algorithm::split;

static command protocol::parse(string data) {

    vector<string> packet;
    command cmd;

    split(packet, data, boost::is_any_of(PROTOCOL_DELIMITER));

    if (packet.size() != PROTOCOL_EXPECTED_SIZE) {
        cmd.is_malformed = true;
    }
    else {
        if (packet[0] != PROTOCOL_HEADER) {
            cmd.is_malformed = true;
        }
        else if (packet[1] == "command" && packet[2] != "") {
            cmd.is_internal = true;
            cmd.executable = packet[1];
            cmd.arguments = packet[2];
        }
        else {
            cmd.executable = packet[1];
            cmd.arguments = packet[2];
            cmd.interval = lexical_cast<unsigned int>(packet[3]);
            cmd.timeout = lexical_cast<unsigned int>(packet[4]);
            cmd.do_not_queue = packet[5] == "1";
            cmd.is_malformed = false;
        }
    }

    return cmd;
    
}

static string protocol::build(const command &cmd) { 

    vector<string> packet;

    packet.push_back(PROTOCOL_HEADER);
    packet.push_back(cmd.executable);
    packet.push_back(cmd.arguments);
    packet.push_back(lexical_cast<string>(cmd.interval));
    packet.push_back(lexical_cast<string>(cmd.timeout));
    packet.push_back(cmd.do_not_queue ? "1" : "0");
    packet.push_back("");

    return join(packet, PROTOCOL_DELIMITER);
    
}

#include "stdafx.h"
#include "command.h"

#ifndef PROTOCOL_H
#define PROTOCOL_H

#define PROTOCOL_HEADER "cgi4lcd"
#define PROTOCOL_DELIMITER "|"
#define PROTOCOL_EXPECTED_SIZE 7

using namespace std;

class protocol {

public:

    static command parse(string data);
    static string build(const command &cmd);

};

#endif // PROTOCOL_H


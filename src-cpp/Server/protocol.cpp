#include "stdafx.h"
#include "command.cpp"
#include <msgpack.hpp>

using std::string;
using std::vector;
using boost::lexical_cast;

#ifndef PROCOTOL_CPP
#define PROTOCOL_CPP

class protocol {

public:

    static command parse(string data) {

		msgpack::unpacked message;
		msgpack::unpack(&message, data.data(), data.size());

        msgpack::object object = message.get();
 
        std::vector<command> rvec;
        object.convert(&rvec);

		return rvec.front();
        
    }

	static char* build(const command &cmd) {

		std::vector<command> container;

		msgpack::sbuffer mpbuffer;
		msgpack::pack(mpbuffer, container);

		return mpbuffer.data();
	}

};

#endif // PROCOTOL_CPP


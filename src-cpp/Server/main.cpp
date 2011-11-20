#include "stdafx.h"
#include "server.cpp"

int main(int argc, char* argv[]) {

    try {

        boost::asio::io_service io_service;

        server s(io_service, std::atoi(argv[1]));

        io_service.run();

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
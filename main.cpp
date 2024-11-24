#include <iostream>
#include <csignal>
#include <cstdlib>  // For atoi()
#include "Server.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if (port <= 0) {
        std::cerr << "Invalid port number." << std::endl;
        return 1;
    }

    std::string password = argv[2];

    Server ser(port, password);
    std::cout << "---- SERVER ----" << std::endl;
    try {
        signal(SIGINT, Server::SignalHandler);  // Catch SIGINT (Ctrl+C)
        signal(SIGQUIT, Server::SignalHandler); // Catch SIGQUIT (Ctrl+\)
        ser.ServerInit(); // Initialize and run the server
    }
    catch (const std::exception& e) {
        ser.CloseFds(); // Close file descriptors
        std::cerr << e.what() << std::endl;
    }
    std::cout << "The Server Closed!" << std::endl;
    return 0;
}
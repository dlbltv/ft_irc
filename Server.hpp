#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>     // For fcntl()
#include <unistd.h>    // For close()
#include <arpa/inet.h> // For inet_ntoa()
#include <poll.h>      // For poll()
#include <cstring>
#include <csignal>     // For signal()
#include <stdexcept>
#include "Client.hpp"
#include "Chanel.hpp"

#define RED "\e[1;31m" // For red color
#define WHI "\e[0;37m" // For white color
#define GRE "\e[1;32m" // For green color
#define YEL "\e[1;33m" // For yellow color

class Server
{
private:
    int Port;                // Server port
    int SerSocketFd;         // Server socket file descriptor
    std::string password;    // Server password
    static bool Signal;      // Static boolean for signal handling
    std::vector<Client> clients;           // Vector of connected clients
    std::vector<struct pollfd> fds;        // Vector for poll()
    std::string serverName;
    std::map<std::string, Chanel*> chanels;

    void SerSocket();                        // Server socket creation
    void AcceptNewClient();                  // Accept new client connections
    void ReceiveNewData(int fd);             // Receive data from clients
    void processMessage(int fd, const std::string& message); // Process client messages
    void parseCommand(Client* client, const std::string& line); // Parse and handle commands
    std::vector<std::string> split(const std::string& s, const std::string& delimiter); // Split a string by delimiter
    Client* getClientByFd(int fd);           // Get a client by file descriptor

public:
    Server(int port, const std::string& pass);
    ~Server();

    void ServerInit();                 // Server initialization

    static void SignalHandler(int signum); // Signal handler
    void deleteChannel(const std::string& channelName); // Delete a channel
    void CloseFds();                    // Close file descriptors

    // Public utilities
    const std::string& getPassword() const;
    Chanel* getChannel(const std::string& name);
    void addChannel(const std::string& name, Chanel* channel);
    std::map<std::string, Chanel*>& getChannels();
    void sendError(int fd, const std::string& code, const std::string& message); 
    void ClearClients(int fd);                                                  
    bool isNicknameUnique(const std::string& nickname);                         
    void sendWelcomeMessage(Client* client); 
    void sendMessage(int fd, const std::string& message);                                   
    Client* getClientByNickname(const std::string& nickname);
    void sendNotice(int fd, const std::string& message);

    const std::string& getServerName() const {
        return serverName;
    }                  
};

#endif // SERVER_HPP

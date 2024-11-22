#include "Server.hpp"
#include <sstream>
#include <algorithm>
#include "Commands.hpp"

// Initialize static member
bool Server::Signal = false;

Server::Server(int port, const std::string& pass) : Port(port), SerSocketFd(-1), password(pass), serverName("MyIRCd")
{
}

Server::~Server()
{
    CloseFds();
}

void Server::ServerInit()
{
    SerSocket(); // Create the server socket

    std::cout << GRE << "Server <" << SerSocketFd << "> Connected on port " << Port << WHI << std::endl;

    while (!Server::Signal) {
        int poll_count = poll(&fds[0], fds.size(), -1);

        if (poll_count == -1 && !Server::Signal) {
            throw std::runtime_error("poll() failed");
        }

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == SerSocketFd)
                    AcceptNewClient(); // Accept new client
                else
                    ReceiveNewData(fds[i].fd); // Receive data from existing client
            }
        }
    }
    CloseFds();
}

void Server::SerSocket()
{
    int en = 1;
    struct sockaddr_in addr;
    struct pollfd NewPoll;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->Port);

    SerSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (SerSocketFd == -1)
        throw std::runtime_error("Failed to create socket");

    if (setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        throw std::runtime_error("Failed to set socket options");

    if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set socket to non-blocking");

    if (bind(SerSocketFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw std::runtime_error("Failed to bind socket");

    if (listen(SerSocketFd, SOMAXCONN) == -1)
        throw std::runtime_error("listen() failed");

    NewPoll.fd = SerSocketFd;
    NewPoll.events = POLLIN;
    NewPoll.revents = 0;
    fds.push_back(NewPoll);
}

void Server::AcceptNewClient()
{
    Client cli;
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    memset(&cliaddr, 0, sizeof(cliaddr));

    int clientfd = accept(SerSocketFd, (struct sockaddr *)&cliaddr, &len);
    if (clientfd == -1) {
        std::cout << "accept() failed" << std::endl; return;
    }

    if (fcntl(clientfd, F_SETFL, O_NONBLOCK) == -1) {
        std::cout << "fcntl() failed" << std::endl; close(clientfd); return;
    }

    struct pollfd NewPoll;
    NewPoll.fd = clientfd;
    NewPoll.events = POLLIN;
    NewPoll.revents = 0;

    cli.SetFd(clientfd);
    cli.setIpAdd(inet_ntoa(cliaddr.sin_addr));
    clients.push_back(cli);
    fds.push_back(NewPoll);

    std::cout << GRE << "Client <" << clientfd << "> Connected" << WHI << std::endl;
}

void Server::ReceiveNewData(int fd)
{
    char buffer[512]; // IRC messages are limited to 512 characters including CRLF
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);

    if (bytes <= 0) {
        std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
        ClearClients(fd);
        close(fd);
    } else {
        buffer[bytes] = '\0';
        std::string message(buffer);
        std::cout << YEL << "Client <" << fd << "> Data: " << WHI << message;

        // Process the message
        processMessage(fd, message);
    }
}

void Server::processMessage(int fd, const std::string& message)
{
    Client* client = getClientByFd(fd);
    if (!client)
        return;

    client->buffer += message;

    size_t pos;
    while ((pos = client->buffer.find("\r\n")) != std::string::npos) {
        std::string line = client->buffer.substr(0, pos);
        client->buffer.erase(0, pos + 2); // Remove processed line and CRLF
        if (!line.empty())
            parseCommand(client, line);
    }
}

void Server::parseCommand(Client* client, const std::string& line) {
    std::istringstream iss(line);
    std::string prefix, command, params;

    if (line[0] == ':') {
        iss >> prefix;
        iss >> command;
    } else {
        iss >> command;
    }

    std::getline(iss, params);
    if (!params.empty() && params[0] == ' ') {
        params.erase(0, 1);
    }

    std::transform(command.begin(), command.end(), command.begin(), ::toupper);


    if (command == "QUIT") {
        handleQuitCommand(*this, client, params);
        return;
    }
    // Allow only authentication commands before authentication is complete
    if (!client->getHasProvidedPassword() || !client->getHasNickname() || !client->getHasUsername()) {
        if (command != "PASS" && command != "NICK" && command != "USER") {
            sendError(client->GetFd(), "451", command + " :You must authenticate before using this command");
            return;
        }
    }

    if (command == "PASS") {
        handlePassCommand(*this, client, params);
    } else if (command == "NICK") {
        std::cout << "Debug: handleNickCommand invoked with nickname: " << params << std::endl;
        handleNickCommand(*this, client, params);
    } else if (command == "USER") {
        handleUserCommand(*this, client, params);
    } else if (command == "JOIN") {
        handleJoinCommand(*this, client, params);
    } else if (command == "PRIVMSG") {
        size_t splitPos = params.find(' ');
        if (splitPos != std::string::npos) {
            std::string target = params.substr(0, splitPos);
            std::string message = params.substr(splitPos + 1);
            handlePrivmsgCommand(*this, client, target, message);
        } else {
            sendError(client->GetFd(), "461", "PRIVMSG :Not enough parameters");
        }
    } else {
        sendError(client->GetFd(), "421", command + " :Unknown command");
    }
}

bool Server::isNicknameUnique(const std::string& nickname) {
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getNickname() == nickname) {
            return false;
        }
    }
    return true;
}

void Server::sendWelcomeMessage(Client* client)
{
    std::string welcomeMsg = ":" + serverName + " 001 " + client->getNickname() + " :Welcome to the IRC server\r\n";
    send(client->GetFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);
}

void Server::sendMessage(int fd, const std::string& message) {
    send(fd, message.c_str(), message.length(), 0);
}

Client* Server::getClientByFd(int fd)
{
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].GetFd() == fd)
            return &clients[i];
    }
    return NULL;
}

void Server::sendError(int fd, const std::string& code, const std::string& message)
{
    std::string errorMsg = ":" + serverName + " " + code + " * " + message + "\r\n";
    send(fd, errorMsg.c_str(), errorMsg.size(), 0);
}

std::vector<std::string> Server::split(const std::string& s, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    if (start < s.length())
        tokens.push_back(s.substr(start));
    return tokens;
}

void Server::ClearClients(int fd) {
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->GetFd() == fd) {
            // Erase client from the vector; do not manually delete it
            clients.erase(it);
            break;
        }
    }

    for (size_t i = 0; i < fds.size(); ++i) {
        if (fds[i].fd == fd) {
            fds.erase(fds.begin() + i);
            break;
        }
    }
    close(fd);
}

void Server::SignalHandler(int signum)
{
    (void)signum;
    std::cout << std::endl << "Signal Received!" << std::endl;
    Server::Signal = true;
}

void Server::deleteChannel(const std::string& channelName) {
    Chanel* channel = chanels[channelName];
    delete channel; // Free the memory for the channel
    chanels.erase(channelName); // Remove the channel from the map
}

void Server::CloseFds() {
    std::string shutdownMsg = ":" + serverName + " NOTICE * :Server shutting down\r\n";

    // Notify all clients about the shutdown and close their connections
    for (size_t i = 0; i < clients.size(); ++i) {
        send(clients[i].GetFd(), shutdownMsg.c_str(), shutdownMsg.size(), 0);

        // Log and close the client's connection
        std::cout << RED << "Client <" << clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
        close(clients[i].GetFd());
    }
    clients.clear(); // Clear the vector of clients

    // Delete all channels and clear the chanels map
    for (std::map<std::string, Chanel*>::iterator it = chanels.begin(); it != chanels.end(); ++it) {
        delete it->second; // Free the memory allocated for the Chanel object
    }
    chanels.clear(); // Clear the map

    // Close the server socket
    if (SerSocketFd != -1) {
        std::cout << RED << "Server <" << SerSocketFd << "> Disconnected" << WHI << std::endl;
        close(SerSocketFd);
    }
}

const std::string& Server::getPassword() const {
    return password;
}

Chanel* Server::getChannel(const std::string& name) {
    std::map<std::string, Chanel*>::iterator it = chanels.find(name);
    if (it != chanels.end()) {
        return it->second; // Return the existing channel
    }
    return NULL; // Return NULL if the channel doesn't exist
}

void Server::addChannel(const std::string& name, Chanel* channel) {
    chanels[name] = channel; // Add the channel to the map
}

std::map<std::string, Chanel*>& Server::getChannels() {
    return chanels;
}

Client* Server::getClientByNickname(const std::string& nickname) {
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getNickname() == nickname) {
            return &(*it); // Return a pointer to the matching client
        }
    }
    return NULL; // Return NULL if no client with the nickname exists
}

void Server::sendNotice(int fd, const std::string& message) {
    std::string noticeMsg = ":" + serverName + " NOTICE * " + message + "\r\n";
    send(fd, noticeMsg.c_str(), noticeMsg.size(), 0);
}
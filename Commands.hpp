#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Server.hpp"

void handlePassCommand(Server& server, Client* client, const std::string& password);
void handleNickCommand(Server& server, Client* client, const std::string& nickname);
void handleUserCommand(Server& server, Client* client, const std::string& params);
void handleJoinCommand(Server& server, Client* client, const std::string& channelName);
void handlePrivmsgCommand(Server& server, Client* client, const std::string& target, const std::string& message);
void handleQuitCommand(Server& server, Client* client, const std::string& message);

#endif // COMMANDS_HPP

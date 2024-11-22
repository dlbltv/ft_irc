#include "Chanel.hpp"
#include <iostream>
#include <map>
#include <string>
#include <unistd.h> // For close()
#include <sys/socket.h> // For send()

Chanel::Chanel(const std::string& name) : name(name) {}

Chanel::~Chanel() {}

const std::string& Chanel::getName() const {
    return name;
}

void Chanel::addMember(Client* client) {
    members[client->GetFd()] = client;
    std::cout << "Client <" << client->getNickname() << "> joined channel " << name << std::endl;
}

void Chanel::removeMember(Client* client) {
    members.erase(client->GetFd());
    std::cout << "Client <" << client->getNickname() << "> left channel " << name << std::endl;
}

void Chanel::broadcast(const std::string& message, Client* sender) {
    for (std::map<int, Client*>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->second != sender) { // Skip the sender
            send(it->first, message.c_str(), message.size(), 0);
        }
    }
}

void Chanel::addOperator(Client* client) {
    operators.insert(client->GetFd());
}

bool Chanel::isOperator(Client* client) {
    return operators.find(client->GetFd()) != operators.end();
}

bool Chanel::isMember(Client* client) const {
    // Iterate through the members map and check if the client exists
    for (std::map<int, Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
        if (it->second == client) {
            return true;
        }
    }
    return false;
}

std::string Chanel::getMemberList() const {
    std::string memberList;

    // Iterate through the members map and build a space-separated list of nicknames
    for (std::map<int, Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
        if (!memberList.empty()) {
            memberList += " ";
        }
        memberList += it->second->getNickname();
    }

    return memberList;
}

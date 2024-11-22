#ifndef CHANEL_HPP
#define CHANEL_HPP

#include <string>
#include <map>
#include <set>
#include "Client.hpp"

class Chanel {
private:
    std::string name;
    std::map<int, Client*> members; // Key: client FD, Value: Client pointer
    std::set<int> operators;       // Operator FDs

public:
    Chanel(const std::string& name);
    ~Chanel();

    const std::string& getName() const;
    void addMember(Client* client);
    void removeMember(Client* client);
    void broadcast(const std::string& message, Client* sender = NULL);
    void addOperator(Client* client);
    bool isOperator(Client* client);
    bool isMember(Client* client) const;
    std::string getMemberList() const;
};

#endif // CHANEL_HPP

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:26:51 by idelibal          #+#    #+#             */
/*   Updated: 2024/11/26 16:15:16 by mortins-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(const std::string& name) : name(name) {}

Channel::~Channel() {}

void	Channel::addMember(Client* client) {
	members[client->getFd()] = client;
	std::cout << "Client <" << client->getNickname() << "> joined channel " << name << std::endl;
}

void	Channel::removeMember(Client* client) {
	members.erase(client->getFd());
	std::cout << "Client <" << client->getNickname() << "> left channel " << name << std::endl;
}

void	Channel::broadcast(const std::string& message, Client* sender) {
	for (std::map<int, Client*>::iterator it = members.begin(); it != members.end(); ++it) {
		if (it->second != sender) // Skip the sender
			send(it->first, message.c_str(), message.size(), 0);
	}
}

void	Channel::addOperator(Client* client) {
	operators.insert(client->getFd());
}

// -----------------------------------Checkers----------------------------------
bool	Channel::isOperator(Client* client) {
	return operators.find(client->getFd()) != operators.end();
}

bool	Channel::isMember(Client* client) const {
	// Iterate through the members map and check if the client exists
	for (std::map<int, Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (it->second == client)
			return true;
	}
	return false;
}

// -----------------------------------Getters-----------------------------------
const std::string&	Channel::getName() const {
	return name;
}

std::string	Channel::getMemberList() const {
	std::string	memberList;

	// Iterate through the members map and build a space-separated list of nicknames
	for (std::map<int, Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (!memberList.empty())
			memberList += " ";
		memberList += it->second->getNickname();
	}

	return memberList;
}
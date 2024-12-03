/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:26:51 by idelibal          #+#    #+#             */
/*   Updated: 2024/11/29 21:19:00 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(const std::string& name) : name(name), inviteOnly(false) {}

Channel::~Channel() {}

void	Channel::addMember(Client* client) {
	members[client->getFd()] = client;
	std::cout << "Client <" << client->getNickname() << "> joined channel " << name << std::endl;
}

void	Channel::removeMember(Client* client) {
	std::cout << "Client <" << client->getNickname() << "> left channel " << name << std::endl;
	if (members.size() == 1)
		std::cout << "Channel " << name << " deleted" << std::endl;
	members.erase(client->getFd());
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

void	Channel::addInvite(const std::string& nickname) {
	inviteList.insert(nickname);
}

void	Channel::removeInvite(const std::string& nickname) {
	inviteList.erase(nickname);
}

// -----------------------------------Checkers----------------------------------
bool	Channel::isOperator(Client* client) {
	return operators.find(client->getFd()) != operators.end();
}

bool	Channel::isMember(Client* client) const {
	// Iterate through the members map and check if the client exists
	// for (std::map<int, Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
	// 	if (it->second == client)
	// 		return true;
	// }
	// return false;

	return members.find(client->getFd()) != members.end();
}

bool	Channel::isInvited(const std::string& nickname) const {
	return inviteList.find(nickname) != inviteList.end();
}

bool	Channel::isInviteOnly() const {
	return inviteOnly;
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

// -----------------------------------Setters-----------------------------------

void	Channel::setInviteOnly(bool status) {
	inviteOnly = status;
}
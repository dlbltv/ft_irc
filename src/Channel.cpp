/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:26:51 by idelibal          #+#    #+#             */
/*   Updated: 2024/12/18 18:17:05 by mortins-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(const std::string& name) : name(name), topic(""), inviteOnly(false), topicRestricted(false), userLimit(-1), inviteList(){}

Channel::~Channel() {}

void	Channel::addMember(Client* client) {
	// If this is the first member, make them the operator
	if (members.empty())
		addOperator(client);
	
	members[client->getFd()] = client;
	std::cout << "Client <" << client->getNickname() << "> joined channel " << name << std::endl;

	// Remove invite if the client was invited
	removeInvite(client->getNickname());
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

void	Channel::removeChannelKey() {
	channelKey.clear();
}

void	Channel::removeUserLimit() {
	userLimit = 0;
}

void	Channel::removeOperator(Client* client) {
	operators.erase(client->getFd());
}

std::string Channel::getModes() const {
	std::string modes = "+";
	if (inviteOnly) modes += "i";
	if (topicRestricted) modes += "t";
	if (!channelKey.empty()) modes += "k";
	if (userLimit > 0) modes += "l";
	return modes;
}

// -----------------------------------Checkers----------------------------------
bool	Channel::isOperator(Client* client) {
	return operators.find(client->getFd()) != operators.end();
}

bool	Channel::isMember(Client* client) const {
	return members.find(client->getFd()) != members.end();
}

bool	Channel::isInvited(const std::string& nickname) const {
	return inviteList.find(nickname) != inviteList.end();
}

bool	Channel::isInviteOnly() const {
	return inviteOnly;
}

bool	Channel::isTopicSet() const {
	if (topic.empty())
		return false;
	return true;
}

bool	Channel::isTopicRestricted() const {
	return topicRestricted;
}

bool	Channel::hasChannelKey() const {
	return !channelKey.empty();
}

bool 	Channel::hasUserLimit() const {
	return userLimit > 0;
}

bool	Channel::isOperator(Client* client) const {
	return operators.find(client->getFd()) != operators.end();
}

// -----------------------------------Getters-----------------------------------
const std::string&	Channel::getName() const {
	return name;
}

const std::string&	Channel::getTopic() const {
	return topic;
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

int	Channel::getUserLimit() const {
	return userLimit;
}

size_t	Channel::getMemberCount() const {
	return members.size();
}

const std::string&	Channel::getChannelKey() const {
	return channelKey;
}

// -----------------------------------Setters-----------------------------------
void	Channel::setTopic(std::string newTopic) {
	topic = newTopic;
}

void	Channel::setInviteOnly(bool status) {
	inviteOnly = status;
}

void	Channel::setTopicRestricted(bool status) {
	topicRestricted = status;
}

void	Channel::setChannelKey(const std::string& key) {
	channelKey = key;
}

void	Channel::setUserLimit(int limit) {
	userLimit = limit;
}
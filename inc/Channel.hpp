/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:26:00 by idelibal          #+#    #+#             */
/*   Updated: 2024/12/13 17:11:27 by mortins-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <set>
# include <iostream>
# include <string>
# include <unistd.h>		// For close()
# include <sys/socket.h>	// For send()
# include "Client.hpp"

class Channel {
	private:
		std::string				name;
		std::string				topic;
		std::map<int, Client*>	members;	// Key: client FD, Value: Client pointer
		std::set<int>			operators;	// Operator FDs
		bool					inviteOnly;  	// Mode: 'i'
		bool 					topicRestricted;// Mode: 't'
		std::string				channelKey;		// Mode: 'k' (password)
		int						userLimit;		// Mode: 'l' (maximum number of users)
		std::set<std::string>	inviteList;

	public:
		Channel(const std::string& name);
		~Channel();

		void	addMember(Client* client);
		void	removeMember(Client* client);
		void	broadcast(const std::string& message, Client* sender = NULL);
		void	addOperator(Client* client);
		void 	addInvite(const std::string& nickname);
		void 	removeInvite(const std::string& nickname);
		void 	removeChannelKey();
		void 	removeUserLimit();
		void 	removeOperator(Client* client);

		std::string	getModes() const;

		// Checkers
		bool	isOperator(Client* client);
		bool	isMember(Client* client) const;
		bool 	isInvited(const std::string& nickname) const;
		bool 	isInviteOnly() const;
		bool	isTopicSet() const;
		bool 	isTopicRestricted() const;
		bool 	hasChannelKey() const;
		bool 	hasUserLimit() const;
		bool	isOperator(Client* client) const;

		// Getters
		const std::string&	getName() const;
		const std::string&	getTopic() const;
		std::string			getMemberList() const;
		int					getUserLimit() const;
		size_t				getMemberCount() const;
		const std::string&	getChannelKey() const;

		
		// Setters
		void	setTopic(std::string newTopic);
		void	setInviteOnly(bool status);
		void	setTopicRestricted(bool status);
		void	setChannelKey(const std::string& key);
		void	setUserLimit(int limit);
		
};

#endif // Channel_HPP
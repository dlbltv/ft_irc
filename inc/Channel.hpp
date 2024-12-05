/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:26:00 by idelibal          #+#    #+#             */
/*   Updated: 2024/12/05 17:43:14 by mortins-         ###   ########.fr       */
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
		std::set<std::string>	inviteList;
		bool					inviteOnly; 

	public:
		Channel(const std::string& name);
		~Channel();

		void	addMember(Client* client);
		void	removeMember(Client* client);
		void	broadcast(const std::string& message, Client* sender = NULL);
		void	addOperator(Client* client);
		void 	addInvite(const std::string& nickname);
		void 	removeInvite(const std::string& nickname);

		// Checkers
		bool	isOperator(Client* client);
		bool	isMember(Client* client) const;
		bool 	isInvited(const std::string& nickname) const;
		bool 	isInviteOnly() const;

		// Getters
		const std::string&	getName() const;
		const std::string&	getTopic() const;
		std::string			getMemberList() const;
		
		// Setters
		void	setTopic(std::string newTopic);
		void	setInviteOnly(bool status);
};

#endif // Channel_HPP
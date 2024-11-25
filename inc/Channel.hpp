/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:26:00 by idelibal          #+#    #+#             */
/*   Updated: 2024/11/25 17:34:57 by mortins-         ###   ########.fr       */
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
		std::string name;
		std::map<int, Client*> members; // Key: client FD, Value: Client pointer
		std::set<int> operators;		// Operator FDs

	public:
		Channel(const std::string& name);
		~Channel();

		const std::string& getName() const;
		void addMember(Client* client);
		void removeMember(Client* client);
		void broadcast(const std::string& message, Client* sender = NULL);
		void addOperator(Client* client);
		bool isOperator(Client* client);
		bool isMember(Client* client) const;
		std::string getMemberList() const;
};

#endif // Channel_HPP
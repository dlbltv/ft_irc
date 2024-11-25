/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:33:21 by idelibal          #+#    #+#             */
/*   Updated: 2024/11/25 17:35:34 by mortins-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"

void handlePassCommand(Server& server, Client* client, const std::string& pass) {
	if (pass.empty()) {
		server.sendError(client->GetFd(), "461", "PASS :Not enough parameters");
		return;
	}
	if (client->getAuthenticationStatus()) {
		server.sendError(client->GetFd(), "462", "PASS :You may not reregister");
		return;
	}
	if (pass != server.getPassword()) { // Assume Server has a `getPassword()` function
		server.sendError(client->GetFd(), "464", "PASS :Password incorrect");
		server.ClearClients(client->GetFd());
		close(client->GetFd());
	} else {
		client->setAuthenticationStatus(true);
		 client->setHasProvidedPassword(true);
		server.sendNotice(client->GetFd(), "Password accepted. Please set your NICK.");
	}
}

void handleNickCommand(Server& server, Client* client, const std::string& nickname) {
	if (!client->getHasProvidedPassword()) {
		server.sendError(client->GetFd(), "451", "NICK :You must provide PASS first");
		return;
	}
	if (nickname.empty()) {
		server.sendError(client->GetFd(), "431", "NICK :No nickname given");
		return;
	}
	if (!server.isNicknameUnique(nickname)) {
		server.sendError(client->GetFd(), "433", nickname + " :Nickname is already in use");
		return;
	}

	client->setNickname(nickname);
	client->setHasNickname(true);
	server.sendNotice(client->GetFd(), "Nickname set. Please provide your USER details.");
}

void handleUserCommand(Server& server, Client* client, const std::string& params) {
	if (!client->getHasProvidedPassword()) {
		server.sendError(client->GetFd(), "451", "USER :You must provide PASS first");
		return;
	}
	if (!client->getHasNickname()) {
		server.sendError(client->GetFd(), "451", "USER :You must set a NICK first");
		return;
	}
	if (params.empty()) {
		server.sendError(client->GetFd(), "461", "USER :Not enough parameters");
		return;
	}

	std::istringstream iss(params);
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);

	if (!realname.empty() && realname[0] == ':') {
		realname.erase(0, 1);
	}

	if (username.empty()) {
		server.sendError(client->GetFd(), "461", "USER :Not enough parameters");
		return;
	}

	client->setUsername(username);
	client->setHasUsername(true);

	if (client->getAuthenticationStatus() && !client->getNickname().empty()) {
		server.sendWelcomeMessage(client);
	}
}

void handleJoinCommand(Server& server, Client* client, const std::string& channelName) {
	if (channelName.empty()) {
		server.sendError(client->GetFd(), "461", "JOIN :Not enough parameters");
		return;
	}

	// Fetch or create the channel
	Channel* channel = server.getChannel(channelName);
	if (!channel) {
		channel = new Channel(channelName);
		server.addChannel(channelName, channel);
	}

	// Check if the client is already a member of the channel
	if (channel->isMember(client)) {
		return; // No need to rejoin
	}

	// Add the client to the channel
	channel->addMember(client);

	// Broadcast the JOIN message to all channel members, including the client
	std::string joinMessage = ":" + client->getNickname() + " JOIN :" + channelName + "\r\n";
	channel->broadcast(joinMessage);

	// Send NAMES list to the client
	std::string namesReply = ":" + server.getServerName() + " 353 " + client->getNickname() +
							 " = " + channelName + " :" + channel->getMemberList() + "\r\n";
	server.sendMessage(client->GetFd(), namesReply);

	// End of NAMES list
	std::string endNamesReply = ":" + server.getServerName() + " 366 " + client->getNickname() +
								" " + channelName + " :End of /NAMES list\r\n";
	server.sendMessage(client->GetFd(), endNamesReply);
}

void handlePrivmsgCommand(Server& server, Client* sender, const std::string& target, const std::string& message) {
	if (target.empty() || message.empty()) {
		server.sendError(sender->GetFd(), "411", "PRIVMSG :No recipient or text to send");
		return;
	}

	if (target[0] == '#') {
		Channel* channel = server.getChannel(target); // Ensure `getChannel` is implemented correctly
		if (!channel) {
			server.sendError(sender->GetFd(), "403", target + " :No such channel");
			return;
		}
		channel->broadcast(":" + sender->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n", sender);
	} else {
		Client* recipient = server.getClientByNickname(target); // This should now link correctly
		if (!recipient) {
			server.sendError(sender->GetFd(), "401", target + " :No such nick");
			return;
		}
		std::string formattedMessage = ":" + sender->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
		send(recipient->GetFd(), formattedMessage.c_str(), formattedMessage.size(), 0);
	}
}

void handleQuitCommand(Server& server, Client* client, const std::string& message) {
	if (!client) {
		std::cerr << "Error: Null client in handleQuitCommand." << std::endl;
		return;
	}

	// Cache client info before removal
	int clientFd = client->GetFd();
	std::string clientNickname = client->getNickname();

	// Construct the QUIT message
	std::string quitMsg = ":" + clientNickname + " QUIT :" + (message.empty() ? "Client exited" : message) + "\r\n";

	// Notify all channels and remove the client
	for (std::map<std::string, Channel*>::iterator it = server.getChannels().begin();
		 it != server.getChannels().end(); ++it) {
		Channel* channel = it->second;

		// Broadcast QUIT to all members except the quitting client
		channel->broadcast(quitMsg, client);

		// Remove the quitting client from the channel
		channel->removeMember(client);
	}

	// Notify the client
	send(clientFd, quitMsg.c_str(), quitMsg.size(), 0);

	// Remove the client from the server
	server.ClearClients(clientFd);
	close(clientFd);
}
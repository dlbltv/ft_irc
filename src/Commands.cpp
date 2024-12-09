/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idlbltv <idlbltv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:33:21 by idelibal          #+#    #+#             */
/*   Updated: 2024/12/09 15:37:12 by idlbltv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Commands.hpp"

void	handlePassCommand(Server& server, Client* client, const std::string& pass) {
	if (pass.empty()) {
		server.sendError(client->getFd(), "461", "PASS :Not enough parameters");
		return;
	}
	if (client->getAuthenticationStatus()) {
		server.sendError(client->getFd(), "462", "PASS :You may not reregister");
		return;
	}
	if (pass != server.getPassword()) { // Assume Server has a `getPassword()` function
		server.sendError(client->getFd(), "464", "PASS :Password incorrect");
		server.clearClients(client->getFd());
		close(client->getFd());
	} else {
		client->setAuthenticationStatus(true);
		 client->setHasProvidedPassword(true);
		server.sendNotice(client->getFd(), "Password accepted. Please set your NICK.");
	}
}

void	handleNickCommand(Server& server, Client* client, const std::string& nickname) {
	if (!client->getHasProvidedPassword()) {
		server.sendError(client->getFd(), "451", "NICK :You must provide PASS first");
		return;
	}
	if (nickname.empty()) {
		server.sendError(client->getFd(), "431", "NICK :No nickname given");
		return;
	}
	if (!server.isNicknameUnique(nickname)) {
		server.sendError(client->getFd(), "433", nickname + " :Nickname is already in use");
		return;
	}

	client->setNickname(nickname);
	client->setHasNickname(true);
	server.sendNotice(client->getFd(), "Nickname set. Please provide your USER details.");
}

void	handleUserCommand(Server& server, Client* client, const std::string& params) {
	if (!client->getHasProvidedPassword()) {
		server.sendError(client->getFd(), "451", "USER :You must provide PASS first");
		return;
	}
	if (!client->getHasNickname()) {
		server.sendError(client->getFd(), "451", "USER :You must set a NICK first");
		return;
	}
	if (params.empty()) {
		server.sendError(client->getFd(), "461", "USER :Not enough parameters");
		return;
	}

	std::istringstream	iss(params);
	std::string			username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);

	if (!realname.empty() && realname[0] == ':')
		realname.erase(0, 1);

	if (username.empty()) {
		server.sendError(client->getFd(), "461", "USER :Not enough parameters");
		return;
	}

	client->setUsername(username);
	client->setHasUsername(true);

	if (client->getAuthenticationStatus() && !client->getNickname().empty())
		server.sendWelcomeMessage(client);
}

void handleJoinCommand(Server& server, Client* client, const std::string& params) {
	std::istringstream iss(params);
	std::string channelName, channelKey;
	iss >> channelName >> channelKey;
	
	if (channelName.empty()) {
		server.sendError(client->getFd(), "461", "JOIN :Not enough parameters");
		return;
	}
	
	// Fetch or create the channel
	Channel*	channel = server.getChannel(channelName);
	if (!channel) {
		channel = new Channel(channelName);
		server.addChannel(channelName, channel);
		std::cout << "Channel " << channelName << " created by client <" << client->getNickname() << ">" << std::endl;
	}

	if (channel->isInviteOnly() && !channel->isInvited(client->getNickname())) {
		server.sendError(client->getFd(), "473", channelName + " :You must be invited to join this channel");
		return;
	}

	// Check user limit
	if (channel->hasUserLimit() && channel->getMemberCount() >= static_cast<size_t>(channel->getUserLimit())) {
		server.sendError(client->getFd(), "471", channelName + " :Cannot join channel (+l)");
		return;
	}

	// Check if the channel has a key set (+k mode)
	if (channel->hasChannelKey()) {
		if (channelKey.empty() || channelKey != channel->getChannelKey()) {
			server.sendError(client->getFd(), "475", channelName + " :Cannot join channel (+k)");
			return;
		}
	}

	// Check if the client is already a member of the channel
	if (channel->isMember(client))
		return; // No need to rejoin

	channel->addMember(client);

	// Broadcast the JOIN message to all channel members, including the client
	std::string joinMessage = ":" + client->getNickname() + " JOIN :" + channelName + "\r\n";
	channel->broadcast(joinMessage);

	// Send NAMES list to the client
	std::string namesReply = ":" + server.getServerName() + " 353 " + client->getNickname() +
							 " = " + channelName + " :" + channel->getMemberList() + "\r\n";
	server.sendMessage(client->getFd(), namesReply);

	// End of NAMES list
	std::string endNamesReply = ":" + server.getServerName() + " 366 " + client->getNickname() +
								" " + channelName + " :End of /NAMES list\r\n";
	server.sendMessage(client->getFd(), endNamesReply);
}

void	handlePrivmsgCommand(Server& server, Client* sender, const std::string& target, const std::string& message) {
	if (target.empty() || message.empty()) {
		server.sendError(sender->getFd(), "411", "PRIVMSG :No recipient or text to send");
		return;
	}

	if (target[0] == '#') {
		Channel*	channel = server.getChannel(target); // Ensure `getChannel` is implemented correctly
		if (!channel) {
			server.sendError(sender->getFd(), "403", target + " :No such channel");
			return;
		}
		// Check if the client is a member of the channel
		if (!channel->isMember(sender)) {
			server.sendError(sender->getFd(), "404", target + " :You are not a member of this channel");
			return;
		}
		channel->broadcast(":" + sender->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n", sender);
	} else {
		Client*	recipient = server.getClientByNickname(target); // This should now link correctly
		if (!recipient) {
			server.sendError(sender->getFd(), "401", target + " :No such nick");
			return;
		}
		std::string formattedMessage = ":" + sender->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
		send(recipient->getFd(), formattedMessage.c_str(), formattedMessage.size(), 0);
	}
}

void	handleQuitCommand(Server& server, Client* client, const std::string& message) {
	if (!client) {
		std::cerr << "Error: Null client in handleQuitCommand." << std::endl;
		return;
	}

	// Cache client info before removal
	int			clientFd = client->getFd();
	std::string	clientNickname = client->getNickname();

	// Construct the QUIT message
	std::string quitMsg = ":" + clientNickname + " QUIT :" + (message.empty() ? "Client exited" : message) + "\r\n";

	// Notify all channels and remove the client
	for (std::map<std::string, Channel*>::iterator it = server.getChannels().begin();
		it != server.getChannels().end(); ++it) {
		Channel* channel = it->second;
		if (channel->isMember(client))
		{
			// Broadcast QUIT to all members except the quitting client
			channel->broadcast(quitMsg, client);

			// Remove the quitting client from the channel
			channel->removeMember(client);
		}
	}

	// Notify the client
	send(clientFd, quitMsg.c_str(), quitMsg.size(), 0);

	std::cout << RED << "Client <" << clientFd << "> Disconnected" << RESET << std::endl;

	// Remove the client from the server
	server.clearClients(clientFd);
	close(clientFd);
}

// Shows available commands
// with '-l' option shows what commands do
void	handleHelpCommand(Server& server, Client* client, const std::string& argument) {
	std::string helpMsg = ":Commands Available:\r\n";
	if (argument.empty()) {
		helpMsg += "\tPASS     NICK     USER     JOIN\r\n\tPRIVMSG  INVITE  QUIT\r\n:For more details type HELP -l\r\n";
		server.sendMessage(client->getFd(), helpMsg);
		return;
	} else if (argument != "-l") {
		server.sendError(client->getFd(), "401", argument + ": No such option");
		return;
	}
	helpMsg += "\t\e[34mPASS\e[0m : Usage: PASS <password>, authenticates you on the server\r\n";
	helpMsg += "\t\e[34mNICK\e[0m : Usage: NICK <nickname>, sets your nick\r\n";
	helpMsg += "\t\e[34mUSER\e[0m : Usage: USER <user_info>, sets your user info\r\n";
	helpMsg += "\t\e[34mJOIN\e[0m : Usage: JOIN <channel>, joins the channel\r\n";
	helpMsg += "\t\e[34mPRIVMSG\e[0m : Usage: PRIVMSG <target> <message>, sends a message to the target (user/channel)\r\n";
	helpMsg += "\t\e[34mINVITE\e[0m : Usage: INVITE <nick> <channel>, invites someone to a channel\r\n";
	helpMsg += "\t\e[34mQUIT\e[0m : Usage: QUIT [<reason>], disconnects from the server\r\n";
	server.sendMessage(client->getFd(), helpMsg);
}

void	handleInviteCommand(Server& server, Client* inviter, const std::string& params) {
	// Split params into target nickname and channel name
	std::istringstream iss(params);
	std::string targetNickname, channelName;
	iss >> targetNickname >> channelName;

	if (!channelName.empty() && channelName[0] != '#') {
		channelName = "#" + channelName;
	}

	// Check if the target nickname and channel name are provided
	if (targetNickname.empty() || channelName.empty()) {
		server.sendError(inviter->getFd(), "461", "INVITE :Not enough parameters");
		return;
	}

	// Check if the target client exists
	Client* targetClient = server.getClientByNickname(targetNickname);
	if (!targetClient) {
		server.sendError(inviter->getFd(), "401", targetNickname + " :No such nick");
		return;
	}
	
	Channel* channel = server.getChannel(channelName);
	if (channel) {
		if (!channel->isMember(inviter)) {
			server.sendError(inviter->getFd(), "442", channelName + " :You're not member of that channel");
			return;
		}
		if (!channel->isOperator(inviter)) {
			server.sendError(inviter->getFd(), "482", channelName + " :You're not a channel operator of that channel");
			return;
		}
		if (channel->isMember(targetClient)) {
			server.sendNotice(inviter->getFd(), targetNickname + " is already a member of " + channelName);
			return;
		}
		// Add the target nickname to the channel's invite list
		channel->addInvite(targetClient->getNickname());
	}

	// Send an invite message to the target client
	std::string inviteMsg = ":" + inviter->getNickname() + " INVITE " + targetNickname + " :" + channelName + "\r\n";
	server.sendMessage(targetClient->getFd(), inviteMsg);

	// Notify the inviter about the successful invitation
	server.sendNotice(inviter->getFd(), "Invitation sent to " + targetNickname + " for channel " + channelName);
}

void	handleModeCommand(Server& server, Client* client, const std::string& params) {
	std::istringstream iss(params);
	std::string channelName, modeString, modeParam;
	iss >> channelName >> modeString >> modeParam;

	Channel* channel = server.getChannel(channelName);

	if (!channel) {
		server.sendError(client->getFd(), "403", channelName + " :No such channel");
		return;
	}

	if (modeString.empty()) {
		server.sendError(client->getFd(), "461", "MODE :Not enough parameters");
		return;
	}

	if (!channel->isOperator(client))
	{
		server.sendError(client->getFd(), "482", channelName + " :You're not a channel operator");
		return;
	}

	bool adding = true; // '+' for adding modes, '-' for removing modes
	for (size_t i = 0; i < modeString.size(); i++) {
		char mode = modeString[i];
		if (mode == '+')
			adding = true;
		else if (mode == '-')
			adding = false;
		else if (mode == 'i')
			channel->setInviteOnly(adding);
		else if (mode == 't')
			channel->setTopicRestricted(adding);
		else if (mode == 'k') {
			if (adding) {
				if (modeParam.empty()) {
					server.sendError(client->getFd(), "461", "MODE :Not enough parameters");
					return;
				}
				channel->setChannelKey(modeParam);
			} else {
				channel->removeChannelKey();
			}
		} else if (mode == 'o') {
			Client* targetClient = server.getClientByNickname(modeParam);
			if (!targetClient) {
				server.sendError(client->getFd(), "401", modeParam + " :No such nick");
				return;
			}
			if (!channel->isMember(targetClient)) {
				server.sendError(client->getFd(), "441", modeParam + " :Client is not a member of the channel");
				return;
			}
			if (adding) {
				channel->addOperator(targetClient);
			} else {
				channel->removeOperator(targetClient);
			}
		} else if (mode == 'l') {
			if (adding) {
				std::stringstream ss(modeParam);
				int userLimit;
				if (!(ss >> userLimit) || userLimit <= 0) {
					server.sendError(client->getFd(), "461", "MODE l:Invalid user limit");
					return;
				}
				// Check against the current number of members
				if (userLimit < static_cast<int>(channel->getMemberCount())) {
					server.sendError(client->getFd(), "471", channelName + " :Cannot set channel user limit higher than the current number of members");
					return;
				}
				channel->setUserLimit(userLimit);
			} else {
				channel->removeUserLimit();
			}
		} else {
			server.sendError(client->getFd(), "472", std::string(1, mode) + " :Unknown mode flag");
			return;
		}
	}
	// Notify the channel about the mode change
	std::string modeMessage = ":" + client->getNickname() + " MODE " + channelName + " " + modeString + " " + modeParam + "\r\n";
	channel->broadcast(modeMessage, client);
}

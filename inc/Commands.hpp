/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:33:54 by idelibal          #+#    #+#             */
/*   Updated: 2024/12/05 19:06:10 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Server.hpp"
# include <sstream>

void	handlePassCommand(Server& server, Client* client, const std::string& password);
void	handleNickCommand(Server& server, Client* client, const std::string& nickname);
void	handleUserCommand(Server& server, Client* client, const std::string& params);
void	handleJoinCommand(Server& server, Client* client, const std::string& params);
void	handlePrivmsgCommand(Server& server, Client* client, const std::string& target, const std::string& message);
void	handleQuitCommand(Server& server, Client* client, const std::string& message);
void	handleHelpCommand(Server& server, Client* client, const std::string& argument);
void	handleInviteCommand(Server& server, Client* inviter, const std::string& params);
void 	handleModeCommand(Server& server, Client* client, const std::string& params);

#endif // COMMANDS_HPP
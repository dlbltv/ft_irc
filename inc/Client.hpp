/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:29:30 by idelibal          #+#    #+#             */
/*   Updated: 2024/11/26 18:05:13 by mortins-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Client.hpp
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client {
	private:
		int			fd;	// File descriptor (use lowercase 'fd')
		std::string	ipAdd;
		std::string	_nickname;
		std::string	_username;
		bool		isAuthenticated;
		bool		hasProvidedPassword;
		bool		hasNickname;
		bool		hasUsername;

	public:
		std::string	buffer;	// For incoming message buffering

		Client();
		~Client();

		// Setters
		void	setFd(int fd);
		void	setIpAdd(const std::string& ipAdd);
		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setAuthenticationStatus(bool status);
		void	setHasProvidedPassword(bool status);
		void	setHasNickname(bool status);
		void	setHasUsername(bool status);

		// Getters
		int			getFd();
		std::string	getIpAdd();
		std::string	getNickname();
		std::string	getUsername();
		bool		getAuthenticationStatus();
		bool 		getHasProvidedPassword();
		bool		getHasNickname();
		bool		getHasUsername();
};

#endif // CLIENT_HPP
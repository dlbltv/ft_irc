/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:29:30 by idelibal          #+#    #+#             */
/*   Updated: 2025/01/06 15:17:33 by idelibal         ###   ########.fr       */
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
		std::string	hostname;
		std::string	servername;
		std::string	realname;

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
		void	setHostname(const std::string& hostname);
		void	setServername(const std::string& servername);
		void	setRealname(const std::string& realname);

		// Getters
		int			getFd();
		// std::string	getIpAdd();
		std::string	getNickname();
		std::string	getUsername();
		bool		getAuthenticationStatus();
		bool 		getHasProvidedPassword();
		bool		getHasNickname();
		bool		getHasUsername();
		const std::string& getHostname() const;
		const std::string& getServername() const;
		const std::string& getRealname() const;
};

#endif // CLIENT_HPP
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:32:00 by idelibal          #+#    #+#             */
/*   Updated: 2024/11/26 16:09:52 by mortins-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() : fd(-1), isAuthenticated(false), hasProvidedPassword(false), hasNickname(false), hasUsername(false) {
}

// Add this definition
Client::~Client() {
	// If there's nothing to clean up, leave it empty
}

// -----------------------------------Setters-----------------------------------
void	Client::setFd(int fd) {
	this->fd = fd;
}

void	Client::setIpAdd(const std::string& ipAdd) {
	this->ipAdd = ipAdd;
}

void	Client::setNickname(const std::string& nickname) {
	Nickname = nickname;
}

void	Client::setUsername(const std::string& username) {
	Username = username;
}

void	Client::setAuthenticationStatus(bool status) {
	isAuthenticated = status;
}

void	Client::setHasProvidedPassword(bool status) {
	hasProvidedPassword = status;
}

void	Client::setHasNickname(bool status) {
	hasNickname = status;
}

void	Client::setHasUsername(bool status) {
	hasUsername = status;
}

// -----------------------------------Getters-----------------------------------
int	Client::getFd() {
	return fd;
}

std::string	Client::getNickname() {
	return Nickname;
}

std::string	Client::getUsername() {
	return Username;
}

bool	Client::getAuthenticationStatus() {
	return isAuthenticated;
}

bool	Client::getHasProvidedPassword() {
	return hasProvidedPassword;
}

bool	Client::getHasNickname() {
	return hasNickname;
}

bool	Client::getHasUsername() {
	return hasUsername;
}
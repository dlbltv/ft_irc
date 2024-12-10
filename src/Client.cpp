/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:32:00 by idelibal          #+#    #+#             */
/*   Updated: 2024/12/04 17:01:09 by idelibal         ###   ########.fr       */
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
	_nickname = nickname;
}

void	Client::setUsername(const std::string& username) {
	_username = username;
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
	return _nickname;
}

std::string	Client::getUsername() {
	return _username;
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
#include "Client.hpp"

Client::Client() : fd(-1), isAuthenticated(false), hasProvidedPassword(false), hasNickname(false), hasUsername(false)
{
}

// Add this definition
Client::~Client()
{
    // If there's nothing to clean up, leave it empty
}

void Client::SetFd(int fd)
{
    this->fd = fd;
}

int Client::GetFd()
{
    return fd;
}

void Client::setIpAdd(const std::string& ipAdd)
{
    this->ipAdd = ipAdd;
}

std::string Client::getNickname() {
    return Nickname;
}

void Client::setNickname(const std::string& nickname) {
    Nickname = nickname;
}

std::string Client::getUsername() {
    return Username;
}

void Client::setUsername(const std::string& username) {
    Username = username;
}

bool Client::getAuthenticationStatus() {
    return isAuthenticated;
}

void Client::setAuthenticationStatus(bool status) {
    isAuthenticated = status;
}

bool Client::getHasProvidedPassword() {
    return hasProvidedPassword;
}

void Client::setHasProvidedPassword(bool status) {
    hasProvidedPassword = status;
}

bool Client::getHasNickname() {
    return hasNickname;
}

void Client::setHasNickname(bool status) {
    hasNickname = status;
}

bool Client::getHasUsername() {
    return hasUsername;
}

void Client::setHasUsername(bool status) {
    hasUsername = status;
}
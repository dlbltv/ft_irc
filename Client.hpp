// Client.hpp
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
private:
    int fd;  // File descriptor (use lowercase 'fd')
    std::string ipAdd;
    std::string Nickname;
    std::string Username;
    bool isAuthenticated;
    bool hasProvidedPassword;
    bool hasNickname;
    bool hasUsername;

public:
    std::string buffer;  // For incoming message buffering

    Client();
    ~Client();

    void SetFd(int fd);
    int GetFd();

    void setIpAdd(const std::string& ipAdd);
    std::string getIpAdd();

    void setNickname(const std::string& nickname);
    std::string getNickname();

    void setUsername(const std::string& username);
    std::string getUsername();

    void setAuthenticationStatus(bool status);
    bool getAuthenticationStatus();

    void setHasProvidedPassword(bool status);
    bool getHasProvidedPassword();

    void setHasNickname(bool status);
    bool getHasNickname();

    void setHasUsername(bool status);
    bool getHasUsername();




};

#endif // CLIENT_HPP
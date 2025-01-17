# IRC Server Project

## Overview
This project is an implementation of an IRC server built in C++. It adheres to the basic principles of the IRC protocol, allowing multiple clients to connect, communicate in channels, and execute various commands such as joining channels, sending private messages, and setting topics. The project was created by **idelibal** and **mortins-**.

## Features
- **User Authentication:** Password-based authentication for clients.
- **Channel Management:** Create, join, and manage channels with features like invite-only, user limits, and channel keys.
- **Messaging:** Support for private messages and broadcast messages within channels.
- **Server Administration:** Operators can manage channels and users with commands like `KICK` and `INVITE`.
- **Command Support:** Implements key IRC commands such as `PASS`, `NICK`, `USER`, `JOIN`, `PRIVMSG`, `MODE`, `TOPIC`, `QUIT`, and more.
- **Signal Handling:** Gracefully handles server shutdown via signals like `SIGINT` and `SIGQUIT`.

## Usage
### Prerequisites
- A C++ compiler supporting C++98.
- Basic understanding of networking and the IRC protocol.

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/dlbltv/ft_irc.git
   cd ft_irc
   ```
2. Compile the project:
   ```bash
   make
   ```

### Running the Server
Run the server executable with the following syntax:
```bash
./ircserv <port> <password>
```
- `<port>`: The port number on which the server will listen.
- `<password>`: The password required for clients to authenticate.

Example:
```bash
./ircserv 6987 my_secure_password
```

### Client Interaction
Clients can connect to the server using any standard IRC client (e.g., HexChat, irssi). They must use the password specified during the server launch.

#### Using NetCat as a Client
NetCat (nc) can also be used to test the server. Here is an example:

1. Start the server:
```bash
./ircserv 6987 my_secure_password
```
2. Open a terminal and connect using NetCat:
```bash
nc localhost 6667
```
3. Authenticate with the server by typing the following commands in the NetCat session:
```bash
PASS my_secure_password
NICK mynickname
USER myusername 0 * :My Real Name
```
4. Once authenticated, you can use other IRC commands like:
```bash
JOIN #mychannel
PRIVMSG #mychannel :Hello, World!
QUIT :Goodbye
```

## File Structure
- **`main.cpp`**: Entry point of the server, handling initial setup and signal handling.
- **`Server.cpp`**: Core server logic, including socket creation, client management, and command parsing.
- **`Client.cpp`**: Represents a connected client with methods for managing their state.
- **`Channel.cpp`**: Represents an IRC channel, including membership and modes.
- **`Commands.cpp`**: Implementation of IRC commands.
- **Makefile**: For building the project.

## Key IRC Commands Implemented
| Command   | Description                                                  |
|-----------|--------------------------------------------------------------|
| `PASS`    | Authenticates a client using the server password.            |
| `NICK`    | Sets the client's nickname.                                  |
| `USER`    | Sets the client's username and other details.                |
| `JOIN`    | Joins a client to a channel.                                 |
| `PRIVMSG` | Sends a private message to a user or channel.                |
| `TOPIC`   | Sets or retrieves the topic of a channel.                    |
| `QUIT`    | Disconnects a client from the server.                        |
| `HELP`    | Provides information on available commands.                  |
| `KICK`    | Removes a client from a channel.                             |
| `INVITE`  | Invites a user to join a channel.                            |
| `LIST`    | Lists all channels or details of a specific channel.         |
| `NAMES`   | Lists all users in a channel.                                |
| `MODE`    | Sets or removes channel/user modes.                          |
| `DIE`     | Shuts down the server (operator command).                    |

## Contributors
- **idelibal** - [GitHub](https://github.com/idelibal)
- **mortins-** - [GitHub](https://github.com/mortinso)

## Acknowledgements
Special thanks to the 42 Network for fostering collaboration and learning.



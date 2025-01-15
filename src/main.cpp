/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortins- <mortins-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:39:26 by idelibal          #+#    #+#             */
/*   Updated: 2025/01/15 17:11:16 by mortins-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdlib>  // For atoi()
#include "../inc/Server.hpp"

int	main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int	port = std::atoi(argv[1]);
	if (port <= 0 || port > 65535) {
		std::cerr << "Invalid port number." << std::endl;
		return 1;
	}

	std::string	password = argv[2];

	Server	ser(port, password);
	std::cout << "---- SERVER ----" << std::endl;
	try {
		signal(SIGINT, Server::signalHandler);  // Catch SIGINT (Ctrl+C)
		signal(SIGQUIT, Server::signalHandler); // Catch SIGQUIT (Ctrl+\)
		ser.serverInit(); // Initialize and run the server
	}
	catch (const std::exception& e) {
		ser.closeFds(); // Close file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	return 0;
}

// Add sections to code
// Comment functions meaning
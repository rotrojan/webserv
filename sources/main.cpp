/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 19:48:40 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 15:20:16 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <iostream>
#include "Config.hpp"
#include "HttpServer.hpp"
#include "server.hpp"

#ifndef CONFIG_FILE_NAME
# define CONFIG_FILE_NAME "webserv.conf"
#endif

static Config	__initConfig(int argc, char **argv)
{
	Config	config;

	if (argc > 2)
		throw (std::runtime_error("Too many arguments"));
	if (argc == 2)
		config.parse(argv[1]);
	else
		config.parse(std::string(WEBSERV_PATH) + CONFIG_FILE_NAME);
	return (config);
}

int	main(int argc, char **argv)
{
	try {
		Config					config;
		std::vector<HttpServer>	serverList;


		config = __initConfig(argc, argv);
		try {
			serverList = createServers(config);
			server(serverList);
		}
		catch (const std::exception &e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
		closeServers(serverList);
		return (EXIT_SUCCESS);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
}

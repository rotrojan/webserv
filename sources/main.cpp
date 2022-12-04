/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 19:48:40 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 14:30:35 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <iostream>
#include <syslog.h>
#include "Config.hpp"
#include "EpollSocket.hpp"
#include "HttpServer.hpp"
#include "server.hpp"

#ifndef CONFIG_FILE_NAME
# define CONFIG_FILE_NAME "webserv.conf"
#endif

int	main(void)
{
	try {
		Config				config;
		std::vector<HttpServer>	serverList;

		config.parse(std::string(WEBSERV_PATH) + CONFIG_FILE_NAME);
		try {
			serverList = createServers(config);
			server(serverList);
		}
		catch (const std::exception &e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
		closeServers(serverList);
		return (0);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
}

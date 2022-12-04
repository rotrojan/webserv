/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   createServers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 19:58:50 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 14:52:24 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include "EpollSocket.hpp"
#include "Config.hpp"
#include "HttpServer.hpp"

using namespace config;

static EpollSocket	createSocket(std::string port)
{
	EpollSocket	server;

	server.createSocket(AF_INET, SOCK_STREAM, port);
	server.setNonBlocking();
	server.setSockOpt(SOL_SOCKET, SO_REUSEADDR, 1);
	server.bindSocket();
	server.listenSocket();
	return (server);
}

static int	checkServers(std::vector<HttpServer> &servers, ServerContext &context)
{
	for (size_t i = 0; i < servers.size(); ++i)
	{
		if (servers[i].context.directives.at("listen") == context.directives.at("listen"))
		{
			if (servers[i].context.directives.at("server_name") == context.directives.at("server_name"))
				return (-1);
			return (0);
		}
	}
	return (1);
}

static void	announce(ServerContext &context)
{
	std::vector<std::string>	listen;
	std::string					serverName;

	listen = context.directives.at("listen");
	serverName = context.directives.at("server_name")[0];
	std::cout << "HttpServer " << serverName << " created" << std::endl;
	std::cout << "listening on " << listen[0] << ":" << listen[1] << std::endl;
}

static EpollSocket	findSocket(std::vector<HttpServer> &servers, ServerContext &context)
{
	for (size_t i = 0; i < servers.size(); ++i)
	{
		if (servers[i].context.directives.at("listen") == context.directives.at("listen"))
			return (servers[i].socket);
	}
	return (EpollSocket());
}

static void	addServer(std::vector<HttpServer> &servers, ServerContext &context)
{
	int			check;
	EpollSocket	socket;

	check = checkServers(servers, context);
	if (check == -1)
		throw std::runtime_error("HttpServer already exists");
	else if (check == 0)
		socket = findSocket(servers, context);
	else
		socket = createSocket(context.directives.at("listen")[1]);
	servers.push_back(HttpServer(socket, context));
	announce(context);
}

std::vector<HttpServer>	createServers(Config &config)
{
	std::vector<HttpServer>	serverList;

	for (size_t i = 0; i < config.servers.size(); ++i)
	{
		try {
			addServer(serverList, config.servers[i]);
		} catch (const std::exception &e) {
			for (size_t j = 0; j < serverList.size(); ++j)
				serverList[j].socket.closeSocket();
			throw (std::runtime_error(e.what()));
		}
		std::cout << std::endl;
	}
	return (serverList);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:19:24 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 14:30:00 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "Config.hpp"
#include "Epoll.hpp"
#include "EpollSocket.hpp"
#include "HttpServer.hpp"
#include "Client.hpp"

extern bool	g_running;

using namespace config;

std::vector<HttpServer>	createServers(Config &config);
void				closeServers(std::vector<HttpServer> &serverList);
void				handleConnection(std::vector<HttpServer> &serverList,
						std::map<int, Client> &clientList, EpollSocket &socketEvent, Epoll &epoll);
void				handleDeconnection(std::map<int, Client> &clientList, EpollSocket &socketEvent, Epoll &epoll);
void				handleInput(std::map<int, Client> &clientList, EpollSocket &socketEvent, Client &client, Epoll &epoll);
void				handleOutput(std::map<int, Client> &clientList, EpollSocket &socketEvent, Client &client, Epoll &epoll);
void				server(std::vector<HttpServer> &serverList);
void				eventLoop(std::vector<HttpServer> &serverList, std::map<int, Client> &clientList,
						Epoll &epoll, int events);

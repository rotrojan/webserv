/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:43:25 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 00:57:12 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

using namespace config;

HttpServer::HttpServer(void) {}

HttpServer::HttpServer(EpollSocket &socket, ServerContext &context) : socket(socket), context(context) {}

HttpServer::HttpServer(const HttpServer &rhs)
{
	*this = rhs;
}

HttpServer::~HttpServer(void) {}

HttpServer	&HttpServer::operator=(const HttpServer &rhs)
{
	if (this != &rhs) {
		this->socket = rhs.socket;
		this->context = rhs.context;
	}
	return (*this);
}

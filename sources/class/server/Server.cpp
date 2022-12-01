/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:43:25 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 00:57:12 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) {}

Server::Server(EpollSocket &socket, ServerContext &context) : socket(socket), context(context) {}

Server::Server(const Server &rhs)
{
	*this = rhs;
}

Server::~Server(void) {}

Server	&Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{
		this->socket = rhs.socket;
		this->context = rhs.context;
	}
	return (*this);
}

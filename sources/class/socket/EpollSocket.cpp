/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSocket.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 23:19:48 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 00:04:29 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EpollSocket.hpp"

EpollSocket::EpollSocket(int fd, uint32_t events): Socket(fd)
{
	bzero(&this->_infoEvent, sizeof(this->_infoEvent));
	this->_infoEvent.data.fd = fd;
	this->_infoEvent.events = events;
}

EpollSocket::EpollSocket(struct epoll_event infoEvent): Socket(infoEvent.data.fd)
{
	bzero(&this->_infoEvent, sizeof(this->_infoEvent));
	this->_infoEvent = infoEvent;
}
	
EpollSocket::EpollSocket(const EpollSocket &socket): Socket(socket)
{
	bzero(&this->_infoEvent, sizeof(this->_infoEvent));
	this->_infoEvent = socket._infoEvent;
}

EpollSocket::~EpollSocket() {}

EpollSocket	&EpollSocket::operator=(const EpollSocket &rhs)
{
	if (this != &rhs)
	{
		Socket::operator=(rhs);
		this->_infoEvent = rhs._infoEvent;
	}
	return (*this);
}

void	EpollSocket::createSocket(int family, int socktype, std::string port, int flags)
{
	Socket::createSocket(family, socktype, port, flags);
	this->_infoEvent.data.fd = this->_listenSocket;
}

void EpollSocket::setEvents(int events)
{
	this->_infoEvent.events = events;
}
	
uint32_t EpollSocket::getEvents(void) const
{
	return (this->_infoEvent.events);
}

struct epoll_event	&EpollSocket::infoEvent(void)
{
	return (this->_infoEvent);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 22:47:38 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 00:04:48 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

Epoll::Epoll(int maxEvents): _maxEvents(maxEvents), _eventList(NULL)
{
	this->_epollFd = epoll_create1(0);
	if (this->_epollFd == -1)
		throw (std::runtime_error(strerror(errno)));
	this->_eventList = new struct epoll_event[maxEvents];
}

Epoll::~Epoll()
{
	close(this->_epollFd);
	delete[] this->_eventList;
}

Epoll	&Epoll::operator=(const Epoll &rhs)
{
	if (this != &rhs)
	{
		this->_epollFd = rhs._epollFd;
		this->_maxEvents = rhs._maxEvents;
		this->_eventList = rhs._eventList;
	}
	return (*this);
}

void	Epoll::control(int option, EpollSocket &target)
{
	if (epoll_ctl(this->_epollFd, option, target.listener(), &target.infoEvent()) == -1)
		throw (std::runtime_error(strerror(errno)));
}

int	Epoll::wait(int timeout)
{
	int	nfds;

	nfds = epoll_wait(this->_epollFd, this->_eventList, this->_maxEvents, timeout);
	if (nfds == -1)
		throw (std::runtime_error(strerror(errno)));
	return (nfds);
}

int	Epoll::pwait(int signums, int timeout)
{
	int			nfds;
	sigset_t	sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, signums);
	nfds = epoll_pwait(this->_epollFd, this->_eventList, this->_maxEvents, timeout, &sigset);
	if (nfds == -1)
		throw (std::runtime_error(strerror(errno)));
	return (nfds);
}

EpollSocket Epoll::socketAt(int index)
{
	if (index < 0 || index >= this->_maxEvents)
		throw (std::out_of_range("Epoll: socketAt() index out of range"));
	return (EpollSocket(this->_eventList[index]));
}

uint32_t	Epoll::eventAt(int index)
{
	if (index < 0 || index >= this->_maxEvents)
		throw (std::out_of_range("Epoll: eventAt() index out of range"));
	return (this->_eventList[index].events);
}

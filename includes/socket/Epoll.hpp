/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 22:47:38 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 00:07:13 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <sys/epoll.h>
#include <exception>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <csignal>
#include "EpollSocket.hpp"

class Epoll
{
private:
	int					_epollFd;
	int					_maxEvents;
	struct epoll_event	*_eventList;

public:
	explicit Epoll(int maxEvents = 10);
	~Epoll();

	Epoll	&operator=(const Epoll &rhs);

	void		control(int option, EpollSocket &target);
	int			wait(int timeout = -1);
	int			pwait(int signums, int timeout = -1);
	EpollSocket socketAt(int index);
	uint32_t	eventAt(int index);
};

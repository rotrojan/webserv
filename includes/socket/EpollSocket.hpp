/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSocket.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 23:19:48 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 00:05:12 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/epoll.h>
#include "Socket.hpp"

class EpollSocket: public Socket
{
private:
	struct epoll_event	_infoEvent;

public:
	explicit EpollSocket(int fd = 0, uint32_t events = 0);
	explicit EpollSocket(struct epoll_event infoEvent);
	EpollSocket(const EpollSocket &socket);
	~EpollSocket();
	
	EpollSocket			&operator=(const EpollSocket &rhs);

	void				createSocket(int family, int socktype, std::string port, int flags = 0);
	void 				setEvents(int events);	
	uint32_t 			getEvents(void) const;
	struct epoll_event	&infoEvent(void);
};

inline bool			operator==(const EpollSocket &lhs, const EpollSocket &rhs)
{
	return (lhs.listener() == rhs.listener() && lhs.getEvents() && rhs.getEvents());
}



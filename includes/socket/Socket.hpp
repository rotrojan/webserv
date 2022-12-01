/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 22:47:17 by lucocozz          #+#    #+#             */
/*   Updated: 2022/11/30 18:55:48 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <utility>
#include <string>

#define RECV_BUFFER 1024

class Socket
{
protected:
	int				_listenSocket;
	struct addrinfo	*_bindAddress;

public:
	explicit Socket(int fd = 0);
	Socket(const Socket &rhs);
	~Socket();

	Socket		&operator=(const Socket &rhs);
	bool		operator==(const Socket &rhs);

	int			listener(void) const;
	void		createSocket(int family, int socktype, std::string port, int flags = AI_PASSIVE);
	void		setNonBlocking(void);
	void		setSockOpt(int level, int optname, int optval);
	void		closeSocket(void);
	int			isOpen(void);
	void		shutdownSocket(int how = SHUT_RDWR);
	void		freeAddrInfo(void);
	void		bindSocket(void);
	void		listenSocket(int backlog = 10);
	int			acceptConnection(void) const;
	std::pair<std::string, int>	recvData(int flags = 0);
	int			sendData(std::string data, int flags = 0);

	std::string	getNameInfo(int flags = 0) const;
	std::string	__getNameInfo(int socketFd, int flags = 0) const;
};

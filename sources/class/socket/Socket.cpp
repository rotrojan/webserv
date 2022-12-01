/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 22:47:17 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 00:37:29 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int fd): _listenSocket(fd), _bindAddress(NULL) {}

Socket::Socket(const Socket &rhs): _listenSocket(0), _bindAddress(NULL) {
	*this = rhs;
}

Socket::~Socket() {}

Socket	&Socket::operator=(const Socket &rhs)
{
	if (this != &rhs)
	{
		this->_listenSocket = rhs._listenSocket;
		this->_bindAddress = rhs._bindAddress;
	}
	return (*this);
}

bool	Socket::operator==(const Socket &rhs)
{
	return (this->listener() == rhs.listener());
}




int	Socket::listener(void) const
{
	return (this->_listenSocket);
}

void	Socket::createSocket(int family, int socktype, std::string port, int flags)
{
	struct addrinfo	hints;

	if (this->_listenSocket != 0)
		throw (std::runtime_error("Socket already created"));
	std::cout << "Creating socket..." << std::endl;
	bzero(&hints, sizeof(hints));
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	hints.ai_flags = flags;
	getaddrinfo(NULL, port.c_str(), &hints, &this->_bindAddress);
	this->_listenSocket = socket(this->_bindAddress->ai_family,
		this->_bindAddress->ai_socktype, this->_bindAddress->ai_protocol);
	if (this->_listenSocket == -1)
		throw (std::runtime_error(strerror(errno)));
}

void	Socket::setNonBlocking(void)
{
	fcntl(this->_listenSocket, F_SETFL, O_NONBLOCK);
}

void	Socket::setSockOpt(int level, int optname, int optval)
{
	if (setsockopt(this->_listenSocket, level, optname, (void*)&optval, sizeof(optval)) == -1)
		throw (std::runtime_error(strerror(errno)));
}
	
void	Socket::closeSocket(void)
{
	this->freeAddrInfo();
	if (this->_listenSocket != 0)
	{
		if (this->isOpen() != -1)
		{
			if (close(this->_listenSocket) == -1)
				throw (std::runtime_error(strerror(errno)));
		}
		this->_listenSocket = 0;
	}
}
	
int		Socket::isOpen(void)
{
	return (fcntl(this->_listenSocket, F_GETFD));
}

void	Socket::shutdownSocket(int how)
{
	if (this->_listenSocket != 0)
	{
		if (this->isOpen() != -1)
		{
			if (shutdown(this->_listenSocket, how) == -1)
				throw (std::runtime_error(strerror(errno)));
		}
	}
}

void	Socket::freeAddrInfo(void)
{
	if (this->_bindAddress != NULL)
		freeaddrinfo(this->_bindAddress);
	this->_bindAddress = NULL;
}


	

void	Socket::bindSocket(void)
{
	if (this->_listenSocket == 0)
		throw (std::runtime_error("Socket have not been created yet"));
	std::cout << "Binding socket to address..." << std::endl;
	if (bind(this->listener(), this->_bindAddress->ai_addr, this->_bindAddress->ai_addrlen) == -1)
	{
		this->closeSocket();
		throw (std::runtime_error(strerror(errno)));
	}
}

void	Socket::listenSocket(int backlog)
{
	std::cout << "Listening..." << std::endl;
	if (listen(this->_listenSocket, backlog) == -1)
		throw (std::runtime_error(strerror(errno)));
}

int	Socket::acceptConnection(void) const
{
	int						socketClient;
	socklen_t				clientLen;
	struct sockaddr_storage	clientAddress;

	std::cout << "Incoming connection..." << std::endl;
	clientLen = sizeof(clientAddress);
	socketClient = accept(this->_listenSocket, (struct sockaddr*)&clientAddress, &clientLen);
	if (socketClient == -1)
		throw (std::runtime_error(strerror(errno)));
	std::cout << "Client is connected as: " << this->__getNameInfo(socketClient) << std::endl;
	return (socketClient);
}

std::pair<std::string, int>	Socket::recvData(int flags)
{
	int							queueSize = 0;
	std::pair<std::string, int> dataPair;
	char						*buffer;
	int							bytesReceived = 0;
		
	if (ioctl(this->_listenSocket, SIOCINQ, &queueSize) == -1)
		queueSize = 2048;
	buffer = new char[queueSize];

	std::cout << "Reading data..." << std::endl;
	bytesReceived = recv(this->_listenSocket, buffer, queueSize, flags);
	if (bytesReceived <= 0) {
		delete[] buffer;
		throw (std::runtime_error(strerror(errno)));
	}
	std::cout << "Received " << bytesReceived << " bytes." << std::endl;
	dataPair = std::make_pair(std::string().append(buffer, bytesReceived), bytesReceived);
	delete[] buffer;
	return (dataPair);
}

int	Socket::sendData(std::string data, int flags)
{
	int	bytesSent = 0;

	std::cout << "Sending data..." << std::endl;
	bytesSent = send(this->_listenSocket, data.c_str(), data.length(), flags);
	if (bytesSent <= 0)
		throw (std::runtime_error(strerror(errno)));
	std::cout << "Sent " << bytesSent << " of " << data.length() << " bytes." << std::endl << std::endl;
	return (bytesSent);
}



std::string	Socket::getNameInfo(int flags) const
{
	return (this->__getNameInfo(this->_listenSocket, flags));
}

std::string	Socket::__getNameInfo(int socketFd, int flags) const
{
	socklen_t				hostLen;
	struct sockaddr_storage	hostAddress;
	char					addressBuffer[128] = {0};

	hostLen = sizeof(hostAddress);
	if (getpeername(socketFd, (struct sockaddr*)&hostAddress, &hostLen) == -1)
		std::cerr << "getpeername(): " << strerror(errno) << std::endl;
	if (getnameinfo((struct sockaddr*)&hostAddress, hostLen, addressBuffer, sizeof(addressBuffer), NULL, 0, flags) == -1)
		std::cerr << "getnameinfo(): " << strerror(errno) << std::endl;
	return (addressBuffer);
}

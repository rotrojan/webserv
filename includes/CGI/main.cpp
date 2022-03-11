/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 22:47:36 by lucocozz          #+#    #+#             */
/*   Updated: 2022/03/10 13:11:47 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../socket/Epoll.hpp"
#include "../socket/Socket.hpp"
#include "../socket/EpollSocket.hpp"
#include "CGI.hpp"
#include "../http/httpResponse.hpp"
#include "../http/httpRequest.hpp"

// #include "../CGI/ClassCGI.hpp"

void	server(EpollSocket &local)
{
	int			nfds;
	Epoll		epoll;
	EpollSocket	socketEvent;

	local.events(EPOLLIN | EPOLLRDHUP);
	epoll.control(EPOLL_CTL_ADD, local);
	while (true)
	{
		nfds = epoll.wait();
		for (int n = 0; n < nfds; ++n)
		{
			socketEvent = epoll.socketAt(n);
			if (socketEvent.listener() == local.listener())
			{
				EpollSocket	client(local.acceptConnection(), EPOLLIN | EPOLLET | EPOLLRDHUP);

				client.setNonBlocking();
				epoll.control(EPOLL_CTL_ADD, client);
			}
			else if (socketEvent.events() & (EPOLLERR | EPOLLRDHUP | EPOLLHUP))
			{
				epoll.control(EPOLL_CTL_DEL, socketEvent);
				socketEvent.closeSocket();
			}
			else if (socketEvent.events() & EPOLLIN)
			{
				std::pair<std::string, int>	data = socketEvent.recvData();
				std::cout << data.first << std::endl;

				//httpRequest/httpResponse
				httpRequest 	request;
				httpResponse	response;

				if (data.second != 0){
						//CGI_startup temporaire
						CGI cgi(NULL, data);
						int cgiRet = 0;
						//httpRequest/httpResponse
						request.treatRequest(data.first);
						if (request.getPath().find(".php") != std::string::npos)
							cgiRet =cgi.CGIStartup(request.getHeaders());
						if (cgiRet == -1)
							std::cerr << "CGI error" << std::endl;
						response.buildResponse(request);
					}
				socketEvent.sendData("HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: Basic realm=\"Access to the staging site\"\r\nConnection: keep-alive\r\nContent-length: 0\r\n\r\n");
			}
		}
	}
}

int	main()
{
	try {
		EpollSocket	local;

		local.createSocket(AF_INET, SOCK_STREAM, "8080");
		local.setSockOpt(SOL_SOCKET, SO_REUSEADDR, 1);
		local.bindSocket();
		local.listenSocket();
		server(local);
		local.closeSocket();
	}
	catch (const std::exception &e) {
		std::cerr << "error: " << e.what() << std::endl;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleOutput.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 21:19:14 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 14:30:35 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "server.hpp"

void	handleOutput(std::map<int, Client> &clientList, EpollSocket &socketEvent, Client &client, Epoll &epoll)
{
	// TODO: send data then remove Epollout flag
	try {
		client.socket.setEvents(client.socket.getEvents() & ~EPOLLOUT);
		epoll.control(EPOLL_CTL_MOD, client.socket);
	}
	catch(std::exception &e) {
		std::cerr << "HandleOutput: " << e.what() << std::endl;
		handleDeconnection(clientList, socketEvent, epoll);
	}
}

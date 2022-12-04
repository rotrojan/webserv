/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleInput.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 21:55:31 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 14:30:19 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "server.hpp"

void	handleInput(std::map<int, Client> &clientList, EpollSocket &socketEvent, Client &client, Epoll &epoll)
{
	std::pair<std::string, int>					data;
	const std::pair<std::string, std::string> 	clientInfo(client.socket.getNameInfo(NI_NUMERICHOST), client.socket.getNameInfo());
	
	try {
		data = client.socket.recvData();
	}
	catch (std::exception &e) {
		std::cerr << "recvData error: " << e.what() << std::endl;
		handleDeconnection(clientList, socketEvent, epoll);
		return ;
	}
	if (client.linkedServer == NULL)
		client.fetchServerlink(data.first);

	// TODO: if all data received, build response and set Epollout flag to send
	// if () {
		// client.socket.setEvents(client.socket.getEvents() | EPOLLOUT);
		// epoll.control(EPOLL_CTL_MOD, client.socket);
	// }
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closeServers.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 22:32:17 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 14:11:06 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

void	closeServers(std::vector<HttpServer> &serverList)
{
	EpollSocket			socket;
	std::vector<int>	closedSockets;

	for (size_t i = 0; i < serverList.size(); ++i)
	{
		socket = serverList[i].socket;
		// avoid double close
		if (std::find(closedSockets.begin(), closedSockets.end(), socket.listener()) == closedSockets.end())
		{
			closedSockets.push_back(socket.listener());
			socket.closeSocket();
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 14:37:30 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 01:04:39 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "Server.hpp"
#include "response.hpp"

class Client
{
private:
	std::vector<Server*>	_serverLinks;

public:
	EpollSocket		socket;
	httpRequest 	request;
	httpResponse	response;
	Server			*linkedServer;

	Client(void);
	Client(EpollSocket &socket, std::vector<Server*> serverLinks);
	Client(const Client &rhs);
	~Client();


	Client	&operator=(const Client &rhs);

	const std::vector<Server*>	&getServerLinks() const;
	void	fetchServerlink(std::string &data);
};



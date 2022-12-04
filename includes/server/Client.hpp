/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 14:37:30 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 13:44:55 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "HttpServer.hpp"

class Client
{
private:
	std::vector<HttpServer*>	_serverLinks;

public:
	EpollSocket		socket;
	HttpServer			*linkedServer;

	Client(void);
	Client(EpollSocket &socket, std::vector<HttpServer*> serverLinks);
	Client(const Client &rhs);
	~Client();


	Client	&operator=(const Client &rhs);

	const std::vector<HttpServer*>	&getServerLinks() const;
	void	fetchServerlink(std::string &data);
};



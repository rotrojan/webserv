/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 14:37:30 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 14:04:40 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void): linkedServer(NULL) {}

Client::Client(EpollSocket &socket, std::vector<HttpServer*> serverLinks):
	_serverLinks(serverLinks), socket(socket), linkedServer(NULL) {}

Client::~Client() {}

Client::Client(const Client &rhs)
{
	*this = rhs;
}

Client	&Client::operator=(const Client &rhs)
{
	if (this != &rhs) {
		this->linkedServer = rhs.linkedServer;
		this->socket = rhs.socket;
		this->_serverLinks = rhs._serverLinks;
	}
	return (*this);
}

const std::vector<HttpServer*>	&Client::getServerLinks() const
{
	return (this->_serverLinks);
}


//! wtf is this shit ??
void	Client::fetchServerlink(std::string &data)
{
	if (this->_serverLinks.size() == 1) {
		this->linkedServer = this->_serverLinks.at(0);
		return ;
	}
	std::string hostName("");
	if (data.find("Host:") != std::string::npos){
		std::string::iterator 	itb;
		std::string::iterator	ite;

		itb = data.begin() + data.find("Host:") + 6;
		ite = itb;
		while (*ite != '\n' && *ite != ':')
			ite++;
		hostName.append(itb, ite);
	}
	for (size_t i = 0; i < this->_serverLinks.size(); ++i)
		if (this->_serverLinks.at(i)->context.directives.find("server_name")->second[0] == hostName){
			this->linkedServer = this->_serverLinks.at(i);
			return ;
		}
	this->linkedServer = this->_serverLinks.at(0);
}

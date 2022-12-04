/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 15:43:25 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 14:41:23 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "EpollSocket.hpp"
#include "Config.hpp"

class HttpServer
{
public:
	EpollSocket				socket;
	config::ServerContext	context;

	HttpServer(void);
	HttpServer(EpollSocket &socket, config::ServerContext &context);
	HttpServer(const HttpServer &rhs);
	~HttpServer(void);

	HttpServer	&operator=(const HttpServer &rhs);
};

inline bool	operator==(const HttpServer &lhs, const HttpServer &rhs)
{
	return (lhs.socket == rhs.socket && lhs.context == rhs.context);
}

inline bool	operator!=(const HttpServer &lhs, const HttpServer &rhs)
{
	return (!(lhs == rhs));
}

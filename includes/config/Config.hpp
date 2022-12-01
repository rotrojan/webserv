/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 01:11:36 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 02:32:39 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <string>
# include "Parser.hpp"

#ifndef DEFAULT_PORT
# define DEFAULT_PORT "8080"
#endif

#ifndef DEFAULT_ROOT
# define DEFAULT_ROOT "/tmp/www/"
#endif

struct LocationContext
{
	std::vector<std::string>							args;
	std::map<std::string, std::vector<std::string> >	directives;
};

inline bool	operator==(const LocationContext &lhs, const LocationContext &rhs)
{
	return (lhs.args == rhs.args && lhs.directives == rhs.directives);
}

struct ServerContext
{
	std::map<std::string, std::vector<std::string> >	directives;
	std::vector<LocationContext>						locations;
};

inline bool	operator==(const ServerContext &lhs, const ServerContext &rhs)
{
	return (lhs.directives == rhs.directives && lhs.locations == rhs.locations);
}

class Config: public Lexer, public Parser
{
private:
	std::string	_filePath;

public:
	std::vector<ServerContext>	servers;

	Config(void);
	Config(const Config &src);
	~Config();

	Config	&operator=(const Config &rhs);

	void	parse(const std::string &filePath);

private:
	void			__checkConfig(void);
	void			__checkDefaultRoot(void);
	void			__checkPortValidity(void);
	std::vector<std::string>	__resolveListen(std::vector<std::string> &listen);
	void			__parsingToData(void);
	ServerContext	__parsedServerToData(const std::vector<Directive> &block);
	LocationContext	__parsedLocationToData(const Directive &directive);
};

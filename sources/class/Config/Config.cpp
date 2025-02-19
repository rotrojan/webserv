/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 01:11:36 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 20:04:25 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

namespace config
{
	Config::Config(void) {}

	Config::Config(const Config &src)
	{
		*this = src;
	}

	Config::~Config() {}

	Config	&Config::operator=(const Config &rhs)
	{
		if (this != &rhs) {
			this->_lexer = rhs._lexer;
			this->_parser = rhs._parser;
			this->_filePath = rhs._filePath;
		}
		return (*this);
	}

	void	Config::parse(const std::string &filePath)
	{
		std::ifstream	fileStream;

		this->_filePath = filePath;
		fileStream.open(filePath.c_str());
		if (fileStream.is_open() == false)
			throw (std::runtime_error("Can't open file: " + filePath));
		this->_lexer.lex(fileStream);
		fileStream.close();
		this->_parser.parse(this->_lexer.getTokens());
		this->__parsingToData();
		this->__checkConfig();
	}



	void	Config::__checkConfig(void)
	{
		this->__checkPortValidity();
		this->__checkDefaultRoot();
	}

	void	Config::__checkDefaultRoot(void)
	{
		for (size_t i = 0; i < this->servers.size(); ++i) {
			if (this->servers[i].directives.find("root") == this->servers[i].directives.end())
				this->servers[i].directives["root"].push_back(DEFAULT_ROOT);
		}
	}

	void	Config::__checkPortValidity(void)
	{
		std::pair<std::vector<std::string>, std::vector<std::string> >					directives;
		std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >	ports;

		for (size_t i = 0; i < this->servers.size(); ++i) {
			this->servers[i].directives["listen"] = this->__resolveListen(this->servers[i].directives["listen"]);
			directives.first = this->servers[i].directives["listen"];
			directives.second = this->servers[i].directives["server_name"];
			if (std::find(ports.begin(), ports.end(), directives) != ports.end())
				throw (std::runtime_error("Duplicate port: " + directives.first[0] + ":" + directives.first[1]));
			ports.push_back(directives);
		}
	}

	std::vector<std::string>	Config::__resolveListen(std::vector<std::string> &listen)
	{
		std::vector<std::string>	resolve;

		if (listen.size() > 2)
			throw (std::runtime_error("Invalid listen directive"));
		if (listen.size() == 0) {
			resolve.push_back("0.0.0.0");
			resolve.push_back(DEFAULT_PORT);
		}
		else if (listen[0].find(".") != std::string::npos) {
			resolve.push_back(listen[0]);
			resolve.push_back(DEFAULT_PORT);
		}
		else {
			resolve.push_back("0.0.0.0");
			resolve.push_back(listen[0]);
		}
		return (resolve);
	}

	void	Config::__parsingToData(void)
	{
		size_t	size = this->_parser.size();

		for (size_t i = 0; i < size; ++i)
			this->servers.push_back(this->__parsedServerToData(this->_parser.getData(i).block));
	}

	ServerContext	Config::__parsedServerToData(const std::vector<Directive> &block)
	{
		ServerContext			server;

		for (size_t i = 0; i < block.size(); ++i) {
			if (block[i].literal == "location")
				server.locations.push_back(this->__parsedLocationToData(block[i]));
			else
				server.directives[block[i].literal] = block[i].args;
		}
		return (server);
	}

	LocationContext	Config::__parsedLocationToData(const Directive &directive)
	{
		LocationContext			location;

		location.args = directive.args;
		for (size_t i = 0; i < directive.block.size(); ++i)
			location.directives[directive.block[i].literal] = directive.block[i].args;
		return (location);
	}
}

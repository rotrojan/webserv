/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.config.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 01:09:40 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 20:12:19 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.config.hpp"

namespace config
{
	Parser::Parser(void)
	{
		this->__readDirectives("server.txt", this->_directivesServer);
		this->__readDirectives("location.txt", this->_directivesLocation);
	}

	Parser::Parser(const Parser &src)
	{
		*this = src;
	}

	Parser::~Parser() {}

	Parser &Parser::operator=(const Parser &rhs)
	{
		if (this != &rhs) {
			this->_directivesServer = rhs._directivesServer;
			this->_directivesLocation = rhs._directivesLocation;
			this->_data = rhs._data;
		}
		return (*this);
	}

	void	Parser::parse(std::vector<Token> &tokens)
	{
		size_t	i = 0;

		this->__checkBrackets(tokens);
		this->_data = this->__parseDirective(tokens, i);
		this->__checkParsing();
	}

	size_t	Parser::size(void)
	{
		return (this->_data.size());
	}

	std::vector<Directive>	&Parser::getData(void)
	{
		return (this->_data);
	}

	Directive	&Parser::getData(uint i)
	{
		return (this->_data[i]);
	}

	void	Parser::__checkParsing(void)
	{
		for (size_t i = 0; i < this->_data.size(); ++i)
		{
			if (this->_data[i].literal == "server")
				this->__checkServerBlock(this->_data[i].block);
			else
				throw (std::runtime_error(this->__errorMsg(this->_data[i].line,
					"Unknow directive: " + this->_data[i].literal)));
		}
	}

	void	Parser::__checkServerBlock(std::vector<Directive> &block)
	{
		for (size_t i = 0; i < block.size(); ++i)
		{
			if (block[i].literal == "location")
				this->__checkLocationBlock(block[i].block);
			else if (this->_directivesServer.find(block[i].literal) == this->_directivesServer.end())
				throw (std::runtime_error(this->__errorMsg(block[i].line,
					"Unknow directive: " + block[i].literal)));
		}
	}

	void	Parser::__checkLocationBlock(std::vector<Directive> &block)
	{
		for (size_t i = 0; i < block.size(); ++i)
		{
			if (this->_directivesLocation.find(block[i].literal) == this->_directivesLocation.end())
				throw (std::runtime_error(this->__errorMsg(block[i].line,
					"Unknow directive: " + block[i].literal)));
		}
	}

	std::vector<Directive>	Parser::__parseDirective(std::vector<Token> &tokens, size_t &i)
	{
		Directive				directive;
		std::vector<Directive>	directiveList;

		for (; i < tokens.size(); ++i)
		{
			directive = Directive();
			if (tokens[i].type == BlockEnd)
				return (directiveList);
			if (tokens[i].type == Keyword)
				directive.literal = tokens[i].literal;
			else
				throw (std::runtime_error(this->__errorMsg(tokens[i].line, "Invalid directive: " + tokens[i].literal)));
			directive.line = tokens[i].line;
			for (++i ; i < tokens.size() && tokens[i].type == Keyword; ++i)
				directive.args.push_back(tokens[i].literal);
			if (tokens[i].type == BlockStart)
				directive.block = this->__parseDirective(tokens, ++i);
			else if (tokens[i].type != Semicolon)
				throw (std::runtime_error(this->__errorMsg(tokens[i].line, "Don't end with ';'")));
			directiveList.push_back(directive);
		}
		return (directiveList);
	}

	void	Parser::__checkBrackets(std::vector<Token> &tokens)
	{
		int	depth = 0;

		for (size_t i = 0; i < tokens.size(); ++i)
		{
			if (tokens[i].type == BlockStart)
				++depth;
			else if (tokens[i].type == BlockEnd)
				--depth;
			if (depth < 0)
				throw (std::runtime_error(this->__errorMsg(tokens[i].line, "unexpected '}'")));
		}
		if (depth > 0)
			throw (std::runtime_error("unexpected end of file, missing '}'"));
	}

	void	Parser::__readDirectives(const std::string &file, std::set<std::string> &directives)
	{
		std::string		line;
		std::ifstream	directivesFile;
		std::string		path(WEBSERV_PATH);

		path += ("directives/" + file);
		directivesFile.open(path.c_str());
		if (directivesFile.is_open() == false)
			throw (std::runtime_error("Can't open directives file: " + path));
		while (directivesFile.eof() == false)
		{
			std::getline(directivesFile, line);
			if (line.empty() == false)
				directives.insert(line);
		}
	}

	std::string	Parser::__errorMsg(const uint line, const std::string message)
	{
		std::stringstream	buffer;

		buffer << line << ": " << message;
		return (buffer.str());
	}
}

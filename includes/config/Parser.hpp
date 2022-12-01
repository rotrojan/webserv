/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 01:09:40 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 01:51:50 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <string>
#include <utility>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "Lexer.hpp"

#ifndef WEBSERV_PATH
# define WEBSERV_PATH NULL
#endif

typedef struct Directive
{
	std::string					literal;
	uint						line;
	std::vector<std::string>	args;
	std::vector<Directive>		block;
} Directive;

class Parser
{
private:
	std::set<std::string>	_directivesServer;
	std::set<std::string>	_directivesLocation;

protected:
	std::vector<Directive>		_parsed;

public:
	Parser(void);
	Parser(const Parser &src);
	~Parser();

	Parser &operator=(const Parser &rhs);

	void	parse(std::vector<Token> &tokens);

private:
	void	__checkParsing(void);
	void	__checkServerBlock(std::vector<Directive> &block);
	void	__checkLocationBlock(std::vector<Directive> &block);
	std::vector<Directive>	__parseDirective(std::vector<Token> &tokens, size_t &i);
	void	__checkBrackets(std::vector<Token> &tokens);
	void	__readDirectives(const std::string &file, std::set<std::string> &directives);
	std::string	__errorMsg(const uint line, const std::string message);
};

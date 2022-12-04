/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 01:06:41 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/04 19:54:50 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.config.hpp"

namespace config
{
	Lexer::Lexer(void) {}

	Lexer::Lexer(const Lexer &src)
	{
		*this = src;
	}

	Lexer::~Lexer() {}

	Lexer &Lexer::operator=(const Lexer &rhs)
	{
		if (this != &rhs)
			this->_tokens = rhs._tokens;
		return (*this);
	}

	void Lexer::lex(std::ifstream &file)
	{
		std::string	line;

		if (file.is_open() == false)
			throw (std::runtime_error("File not open"));
		for (uint lineNumber = 1; file.eof() == false; ++lineNumber)
		{
			std::getline(file, line);
			this->__tokenize(line, lineNumber);
		}
	}

	std::vector<Token>	&Lexer::getTokens(void)
	{
		return (this->_tokens);
	}

	void	Lexer::__tokenize(const std::string &line, const uint &lineNumber)
	{
		std::string	literal;
		uint		column = 0;

		while (column < line.size())
		{
			if (line[column] == '#')
				break;
			else if (line[column] == '{')
				this->__addToken(Token(BlockStart, "{", lineNumber), column);
			else if (line[column] == '}')
				this->__addToken(Token(BlockEnd, "}", lineNumber), column);
			else if (line[column] == ';')
				this->__addToken(Token(Semicolon, ";", lineNumber), column);
			else if (this->__isKeyword(line[column]) == true)
			{
				literal = this->__getKeyword(line, column);
				this->_tokens.push_back(Token(Keyword, literal, lineNumber));
			}
			else
				++column;
		}
	}

	void	Lexer::__addToken(const Token &token, uint &column)
	{
		this->_tokens.push_back(token);
		++column;
	}

	std::string	Lexer::__getKeyword(const std::string &line, uint &column)
	{
		std::string	keyword;

		while (column < line.size() && this->__isKeyword(line[column]) == true)
		{
			keyword += line[column];
			++column;
		}
		return (keyword);
	}

	bool	Lexer::__isKeyword(const char &c)
	{
		const std::string	charset = "_./$-*:";

		if (std::isalnum(c) > 0 || charset.find(c) != std::string::npos)
			return (true);
		return (false);
	}
}

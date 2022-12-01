/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 01:06:41 by lucocozz          #+#    #+#             */
/*   Updated: 2022/12/01 02:21:37 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>

typedef unsigned int	uint;

typedef enum	TokenType
{
	Keyword,	//Keyword: a-zA-Z0-9_./
	BlockStart,	//BlockStart: {
	BlockEnd,	//BlockEnd: }
	Semicolon	//Semicolon: ;
}				TokenType;

class	Token
{
public:
	TokenType	type;
	std::string	literal;
	uint		line;

	Token(TokenType type, const std::string &literal, uint line)
		: type(type), literal(literal), line(line) {}

	~Token() {}
};

class Lexer
{
protected:
	std::vector<Token>	_tokens;

public:
	Lexer(void);
	Lexer(const Lexer &src);
	~Lexer();

	Lexer &operator=(const Lexer &rhs);

	void lex(std::ifstream &file);

private:
	void		__tokenize(const std::string &line, const uint &lineNumber);
	void		__addToken(const Token &token, uint &column);
	std::string	__getKeyword(const std::string &line, uint &column);
	bool		__isKeyword(const char &c);
};

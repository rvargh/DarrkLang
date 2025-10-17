#pragma once

#include "../include/common.h"

/*
* The lexer returns token [0-255] if its an unknown character, 
* otherwise one of these known things
*/

enum Token {

	TOK_EOF = -1,

	//commands
	TOK_DEF = -2,
	TOK_EXTERN = -3,

	//primary
	TOK_IDENTIFIER = -4,
	TOK_NUMBER = -5
};

static std::string identifierStr; //for tok_identifier
static double numVal;

static int getToken()
{
	static int lastChar = ' ';

	while (isspace(lastChar))
	{
		lastChar = getchar();
	}
	std::cout << "i am characters1 : " << (char)lastChar << std::endl;
	// identifier: [a-zA-Z][a-zA-Z0-9]*
	// checks for keywords like def
	if (isalpha(lastChar)) {
		identifierStr = lastChar;

		while (isalnum(lastChar = getchar()))
			identifierStr += lastChar;

		if (identifierStr == "def")
			return TOK_DEF;

		if (identifierStr == "extern")
			return TOK_EXTERN;

		return TOK_IDENTIFIER;
	}
	// number 0-9
	if (isdigit(lastChar) || lastChar == '.') {
		std::string numStr;

		do {
			numStr += lastChar;
			lastChar = getchar();
		} while (isdigit(lastChar) || lastChar == '.');

		// string to double
		// "123.45abc" => 123.45
		numVal = strtod(numStr.c_str(), 0);

		return TOK_NUMBER;
	}
	// handles comments in code
	if (lastChar == '#') {
		do
			lastChar = getchar();
		while (lastChar != EOF &&
			lastChar != '\n' && lastChar != '\r');

		if (lastChar != EOF)
			return getToken();
	}
	// Check for end of file.  Don't eat the EOF.
	if (lastChar == EOF) {
		return TOK_EOF;
	}
	// Otherwise, just return the character as its ascii value.
	int thisChar = lastChar;
	
	lastChar = getchar();
	std::cout << "i am characters2 : " << (char)lastChar << std::endl;
	
	return thisChar;
}

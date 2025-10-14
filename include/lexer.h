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

static int getTok()
{
	static int lastChar = ' ';

	while (isspace(lastChar)) lastChar = getchar();

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

	if (isdigit(lastChar) || lastChar == '.') {
		std::string numStr;

		do {
			numStr += lastChar;
			lastChar = getchar();
		} while (isdigit(lastChar) || lastChar == '.');

		numVal = strtod(numStr.c_str(), 0);

		return TOK_NUMBER;
	}
}

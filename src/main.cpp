#include "../include/lexer.h"
#include "../include/parser.h"

// Binary operation precedence - This holds the precedence for each binary operator that is
static std::map<char, int> binaryOperatioPrecedence;

// Get the precedence of the pending binary operator token
static int getTokenPrecedence()
{
	if (!isascii(currentToken)) return -1;

	int tokenPrecedence = binaryOperatioPrecedence[currentToken];
	if (tokenPrecedence <= 0) return -1;
	return tokenPrecedence;
}

int main() {

	// Install standard binary operators.
	// 1 is lowest precedence.

	binaryOperatioPrecedence['<'] = 10;
	binaryOperatioPrecedence['+'] = 20;
	binaryOperatioPrecedence['-'] = 20;
	// highest
	binaryOperatioPrecedence['*'] = 40;

	enum Token myVar = TOK_NUMBER;


	
	std::cout << "i am here ";
}
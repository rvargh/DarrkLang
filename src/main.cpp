#include "../include/lexer.h"
#include "../include/parser.h"

static void mainLoop() {
	while (true) {
		fprintf(stderr, "readyyyyy>");

		switch (currentToken) {
		case TOK_EOF:
			return;
		case ';':
			getNextToken();
			break;
		case TOK_DEF:
			handleDefinition();
			break;
		case TOK_EXTERN:
			handleExtern();
			break;
		default:
			handleTopLevelExpression();
			break;
		}
	}
}

int main() {

	// Install standard binary operators.
	// 1 is lowest precedence.

	binaryOperatioPrecedence['<'] = 10;
	binaryOperatioPrecedence['+'] = 20;
	binaryOperatioPrecedence['-'] = 20;
	// highest
	binaryOperatioPrecedence['*'] = 40;
	// Prime the first token.
	fprintf(stderr, "ready> ");
	getNextToken();

	// Run the main "interpreter loop" now.
	mainLoop();

}
#include "../include/lexer.h"
#include "../include/parser.h"

// Binary operation precedence - This holds the precedence for each binary operator that is
static std::map<char, int> binaryOperatioPrecedence;
//static std::array<char, 4> binaryOperatioPrecedence;


// Get the precedence of the pending binary operator token
static int getTokenPrecedence()
{
	if (!isascii(currentToken)) return -1;

	int tokenPrecedence = binaryOperatioPrecedence[currentToken];
	if (tokenPrecedence <= 0) return -1;
	return tokenPrecedence;
}

static std::unique_ptr<ExprAst> parseBinaryOperationsRHS(int exprPrecedence, std::unique_ptr<ExprAst> LHS) {
	while (true) {
		int tokenPrecedence = getTokenPrecedence();

		if (tokenPrecedence < exprPrecedence)
			return LHS;

		int binOperation = currentToken;
		getNextToken();

		auto RHS = parsePrimary();
		if (!RHS) {
			return nullptr;
		}

		int nextPrecedence = getTokenPrecedence();
		if (tokenPrecedence < nextPrecedence) {
			RHS = parseBinaryOperationsRHS(tokenPrecedence + 1, std::move(RHS));
			if (!RHS) {
				return nullptr;
			}

		}
		LHS = std::make_unique<BinaryExprAst>(binOperation,
			std::move(LHS), std::move(RHS));
	}
}



static std::unique_ptr<ProtoTypeAst> parseProtoType() {
	if (currentToken != TOK_IDENTIFIER) {
		return logErrorProto("Expected function name is protoype");
	}

	std::string fnName = identifierStr;
	getNextToken();

	if (currentToken != '(') {
		return logErrorProto("Expected '(' in protoype");
	}

	std::vector<std::string> argsNames;
	while (getNextToken() == TOK_IDENTIFIER) {
		argsNames.push_back(identifierStr);
	}

	if (currentToken != ')') {
		return logErrorProto("Expected ')' in protoype");
	}

	getNextToken();

	return std::make_unique<ProtoTypeAst>(fnName, std::move(argsNames));
}

static std::unique_ptr<FunctionAst> parseDefinition() {
	getNextToken();

	auto proto = parseProtoType();
	if (!proto) {
		return nullptr;
	}

	if (auto E = parseExpression()) {
		return std::make_unique<FunctionAst>(std::move(proto), std::move(E));
	}
	return nullptr;
}

static std::unique_ptr<ProtoTypeAst> parseExternal() {
	getNextToken();
	return parseProtoType();
}


static std::unique_ptr<FunctionAst> parseTopLevelExpression() {
	if (auto E = parseExpression()) {
		auto proto = std::make_unique<ProtoTypeAst>("", std::vector<std::string>());
		return std::make_unique<FunctionAst>(std::move(proto), std::move(E));
	}
}
static void handleExtern() {
	if (parseExternal()) {
		fprintf(stderr, "Parsed an extern\n");
	}
	else {
		// Skip token for error recovery.
		getNextToken();
	}
}

static void handleTopLevelExpression() {
	// Evaluate a top-level expression into an anonymous function.
	if (parseTopLevelExpression()) {
		fprintf(stderr, "Parsed a top-level expr\n");
	}
	else {
		// Skip token for error recovery.
		getNextToken();
	}
}

static void handleDefinition() {
	if (parseDefinition()) {
		fprintf(stderr, "Parsed a function definition.\n");
	}
	else {
		// Skip token for error recovery.
		getNextToken();
	}
}

static void mainLoop() {
	while (true) {
		fprintf(stderr, "readyy>");

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
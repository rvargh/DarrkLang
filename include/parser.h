#include"../include/common.h"
#include"../include/ast.h"
#include"../include/lexer.h"

static std::unique_ptr<ExprAst> parseExpression();
static std::unique_ptr<ExprAst> parseBinaryOperationsRHS(int exprPrecedence, std::unique_ptr<ExprAst> LHS);
static int getTokenPrecedence();

// Binary operation precedence - This holds the precedence for each binary operator that is
static std::map<char, int> binaryOperatioPrecedence;
//static std::array<char, 4> binaryOperatioPrecedence;


std::unique_ptr<VariableExprAst> LHS = std::make_unique<VariableExprAst>("x");
std::unique_ptr<VariableExprAst> RHS = std::make_unique<VariableExprAst>("y");
std::unique_ptr<BinaryExprAst> result = std::make_unique<BinaryExprAst>('+', std::move(LHS), std::move(RHS));

static int currentToken;

static int getNextToken()
{
	return currentToken = getToken();
}

std::unique_ptr<ExprAst> logError(const char* str)
{
	fprintf(stderr, "Error: %s\n", str);
	return nullptr;
}

std::unique_ptr<ProtoTypeAst> logErrorProto(const char* str)
{
	logError(str);
	return nullptr;
}

static std::unique_ptr<ExprAst> parseNumberExpression()
{
	auto result = std::make_unique<NumberExprAst>(numVal);
	getNextToken();
	return std::move(result);
}

static std::unique_ptr<ExprAst> parseParanthesisExpression()
{
	getNextToken();
	auto v = parseExpression();
	if (!v) return nullptr;

	if (currentToken != ')')
		return logError("Expected ')'");
	getNextToken();
	return v;
}

static std::unique_ptr<ExprAst> parseIdenifierExpression()
{
	std::string idName = identifierStr;

	getNextToken();
	if (currentToken != '(') {
		return std::make_unique<VariableExprAst>(idName);
	}

	getNextToken();

	std::vector<std::unique_ptr<ExprAst>> args;
	if (currentToken == ')') {
		while (true) {
			if (auto arg = parseExpression())
				args.push_back(std::move(arg));
			else
				return nullptr;

			if (currentToken == ')')
				break;

			if (currentToken != ',')
				return logError("Expected ')' or ',' in argument list");
			getNextToken();
		}
	}

	getNextToken();

	return std::make_unique<CallExptAst>(idName, std::move(args));
}

static std::unique_ptr<ExprAst> parsePrimary() {
	switch (currentToken) {
	default:
		return logError("unknown token when expecting an expression");
	case TOK_IDENTIFIER:
		return parseIdenifierExpression();
	case TOK_NUMBER:
		return parseNumberExpression();
	case '(':
		return parseParanthesisExpression();
	}
}

static std::unique_ptr<ExprAst> parseExpression() {
	auto LHS = parsePrimary();
	if (!LHS)
		return nullptr;

	return parseBinaryOperationsRHS(0, std::move(LHS));
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

// Get the precedence of the pending binary operator token
static int getTokenPrecedence()
{
	if (!isascii(currentToken)) return -1;

	int tokenPrecedence = binaryOperatioPrecedence[currentToken];
	if (tokenPrecedence <= 0) return -1;
	return tokenPrecedence;
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

	return nullptr;
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

#include"../include/common.h"
#include"../include/ast.h"
#include"../include/lexer.h"


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

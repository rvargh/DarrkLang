#include "../include/common.h"

/// ExprAST - Base class for all expression nodes.
class ExprAst {
public:
	virtual ~ExprAst() = default;
	
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAst : public ExprAst {
	double val;

public:
	NumberExprAst(double value) : val(value) { }
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAst : public ExprAst {
	std::string name;
public:
	VariableExprAst(const std::string& nm) : name(nm) { }
};

// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAst : public ExprAst {
	char operation;
	std::unique_ptr<ExprAst> LHS, RHS;

public:
	BinaryExprAst(char op,auto lhs,auto rhs)
		:operation(op),LHS(std::move(lhs)),RHS(std::move(rhs)) { }
};

/// CallExprAST - Expression class for function calls.
class CallExptAst : public ExprAst {
	std::string callee;
	std::vector<std::unique_ptr<ExprAst>> args;
public:
	CallExptAst(const std::string& caller, auto args)
		: callee(caller),args(std::move(args)) { }
};

class ProtoTypeAst {
	std::string protoName;
	std::vector<std::string> protoArgs;
	
public:
	ProtoTypeAst(const std::string& name, auto args)
		:protoName(name),protoArgs(std::move(args)) { }

	const std::string& getName() const {
		return protoName;
	}
};

class FunctionAst {
	std::unique_ptr<ProtoTypeAst> functionProto;
	std::unique_ptr<ExprAst> functionBody;

public:
	FunctionAst(std::unique_ptr<ProtoTypeAst> proto,
		std::unique_ptr<ExprAst> body)
		:functionProto(std::move(proto)),functionBody(std::move(body)) { }
};

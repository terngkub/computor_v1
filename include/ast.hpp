#pragma once
#include "token.hpp"
#include <map>
#include <memory>
#include <string>

struct INode
{
public:
	~INode() {};
	virtual void print() {};
};

template <typename T>
bool is_type(std::shared_ptr<INode> node)
{
	return (std::dynamic_pointer_cast<T>(node) != nullptr) ? true : false;
}

struct TermNode : public INode
{
	std::string	name;
	double		coef;
	int			power;

	TermNode(std::string name);
	TermNode(double coef);
	TermNode(double coef, int power);

	void print();
};

std::ostream & operator<<(std::ostream & o, TermNode const & t);

struct OperationNode : public INode
{
	enum TokenType			op;
	std::shared_ptr<INode>	left;
	std::shared_ptr<INode>	right;

	OperationNode(enum TokenType, std::shared_ptr<INode>, std::shared_ptr<INode>);

	void print();
};

struct ErrorNode : public INode
{
	std::string message;

	ErrorNode(std::string message);
};

struct ExpressionNode : public INode
{
	std::map<int, double> term_map;
	std::string var_name;
	void print();
};
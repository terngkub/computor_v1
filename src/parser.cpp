#include "parser.hpp"

Parser::Parser(Lexer & lexer) :
	lexer(lexer),
	current_token(this->lexer.get_next_token())
{}

INode * Parser::parse()
{
	return expression();
}

// PLUS, MINUS
INode * Parser::expression()
{
	INode * node = term();

	while (current_token->type == TokenType::PLUS
			|| current_token->type == TokenType::MINUS)
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = term();
		node = new OperationNode(token_type, node, right);
	}
	return node;
}

// MULTIPLY, DIVIDE, MODULO
INode * Parser::term()
{
	INode * node = power();

	while (current_token->type == TokenType::MULTIPLY
			|| current_token->type == TokenType::DIVIDE
			|| current_token->type == TokenType::MODULO)
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = power();
		node = new OperationNode(token_type, node, right);
	}
	return node;
}

// POWER
INode * Parser::power()
{
	INode * node = factor();

	while (current_token->type == TokenType::POWER)
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = factor();
		node = new OperationNode(token_type, node, right);
	}
	return node;
}

// NUMBER, VARIABLE
INode * Parser::factor()
{
	INode * node = nullptr;
	if (current_token->type == TokenType::NUMBER)
	{
		node = new TermNode(boost::get<double>(current_token->value));
		current_token = lexer.get_next_token();
	}
	else if (current_token->type == TokenType::VARIABLE)
	{
		node = new TermNode(boost::get<std::string>(current_token->value));
		current_token = lexer.get_next_token();
	}
	else if (current_token->type == TokenType::LPAREN)
	{
		current_token = lexer.get_next_token();
		node = expression();
		if (current_token->type != RPAREN) {
			std::cerr << "Unmatch parenthesis";
			exit(1);
		}
		current_token = lexer.get_next_token();
	}
	return node;
}

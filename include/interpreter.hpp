#pragma once
#include "ast.hpp"
#include <string>
#include <unordered_map>
#include <memory>

class Interpreter
{
    std::shared_ptr<INode> ast;

    INodePtr visit(INodePtr node);
    INodePtr operate(OptPtr op_node, ExprPtr left, ExprPtr right);
    void solve(ExprPtr node);
    void solve_polynomial(ExprPtr node);

public:
    Interpreter(std::shared_ptr<INode>);
    void interpret();

};
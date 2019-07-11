#include "interpreter.hpp"
#include "math.hpp"

Interpreter::Interpreter(std::shared_ptr<INode> ast) :
    ast(ast)
{}

std::shared_ptr<INode> Interpreter::visit(std::shared_ptr<INode> node)
{
    if (std::dynamic_pointer_cast<TermNode>(node) != nullptr)
        return node;
    auto op_node = std::dynamic_pointer_cast<OperationNode>(node);

    auto left = std::dynamic_pointer_cast<TermNode>(visit(op_node->left));
    auto right = std::dynamic_pointer_cast<TermNode>(visit(op_node->right));

    switch(op_node->op)
    {
        case TokenType::PLUS:
            update_term_map(left, true);
            update_term_map(right, true);
            break;

        case TokenType::MINUS:
            update_term_map(left, true);
            update_term_map(right, false);
            break;

        case TokenType::MULTIPLY:
        {
            auto coef = left->coef * right->coef;
            auto power = left->power + right->power;
            return std::make_shared<TermNode>(coef, power);
        }

        case TokenType::DIVIDE:
        {
            if (right->coef == 0)
                throw std::runtime_error("divide by zero");
            auto coef = left->coef / right->coef;
            auto power = left->power - right->power;
            return std::make_shared<TermNode>(coef, power);
        }

        case TokenType::POWER:
            if (left->power == 0 and right->power == 0)
            {
                auto coef = power(left->coef, right->coef);
                return std::make_shared<TermNode>(coef, 0);

            }
            else if (left->power != 0 and right->power == 0)
            {
                return std::make_shared<TermNode>(left->coef, left->power * right->coef);
            }
            else
            {
                // error
            }
            break;
        
        case TokenType::MODULO:
            break;

        case TokenType::EQUAL:
            update_term_map(left, true);
            update_term_map(right, false);
            break;

        default:
            throw std::runtime_error("unsupported operation");
    }
    return nullptr;
}

std::shared_ptr<INode> Interpreter::operate(std::shared_ptr<INode> left, std::shared_ptr<INode> right)
{
    if (is_type<ErrorNode>(left) || is_type<ErrorNode>(right))
    {
        // TODO handle error
        return std::make_shared<ErrorNode>("error");
    }

    // if one of them is error return error
    return nullptr;
}

std::shared_ptr<INode> Interpreter::plus(std::shared_ptr<INode> left, std::shared_ptr<INode> right)
{
    return nullptr;
}


void Interpreter::update_term_map(std::shared_ptr<TermNode> node, bool is_plus)
{
    // TODO can it be case that node == nullptr, should I handle error instead of return nothing?
    if (node == nullptr)
        return ;
    
    if (term_map.find(node->power) == term_map.end())
    {
        if (is_plus)
            term_map[node->power] = node->coef;
        else
            term_map[node->power] = -node->coef;
    }
    else
    {
        if (is_plus)
            term_map[node->power] += node->coef;
        else
            term_map[node->power] -= node->coef;
    }
}

void Interpreter::print_map()
{
    for (auto it = term_map.begin(); it != term_map.end(); ++it)
        std::cout << it->first << ": " << it->second << '\n';
}

void Interpreter::solve()
{
    if (!is_valid_equation())
        return ;
    int max_degree = (*term_map.rbegin()).first;
    if (max_degree == 2)
        solve_polynomial_equation();
    else if (max_degree == 1)
        solve_linear_equation();
    else if (max_degree == 0)
        std::cout << (*term_map.rbegin()).second << '\n';
}

bool Interpreter::is_valid_equation()
{
    if ((*term_map.rbegin()).first > 2)
    {
        std::cerr << "highest degree is more than 2\n";
        return false;
    }
    if ((*term_map.begin()).first < 0)
    {
        std::cerr << "lowest degree is less than 0\n";
        return false;
    }
    return true;
}

void Interpreter::solve_polynomial_equation()
{
    double a = term_map[2];
    double b = term_map[1];
    double c = term_map[0];
    double ret_one = (-b + sqrt(power(b, 2) - 4 * a * c)) / (2 * a);
    double ret_two = (-b - sqrt(power(b, 2) - 4 * a * c)) / (2 * a);
    std::cout << "x = " << ret_one << ", " << ret_two << '\n';
}

void Interpreter::solve_linear_equation()
{
    if (term_map.find(0) != term_map.end())
        std::cout << "x = " << -term_map[0] / term_map[1] << '\n';
    else
        std::cout << "x = 0\n";
}

void Interpreter::interpret()
{
    auto node = std::dynamic_pointer_cast<TermNode>(visit(ast));
    if (node != nullptr)
        update_term_map(node, true);
    print_map();
    solve();
}
#pragma once

#ifndef PARSER_HXX
#define PARSER_HXX

#include <cstdint>
#include <stack>
#include <array>
#include <span>
#include <functional>
#include <memory>

#include "AST.hxx"
#include "token.hxx"

namespace simple_calc
{

class Parser
{
protected:
    enum class State
    {
        statement,
        add_sub,
        add_sub_tail,
        mul_div,
        mul_div_tail,
        operand,
    };
    std::stack<State> m_state{};
    std::span<Token const> m_input_tokens{};
    std::shared_ptr<AST::ASTNode> m_ast_root{};

public:
    explicit Parser(
        std::span<Token const> input_tokens
    ) noexcept
        : m_input_tokens(input_tokens)
    {
        this->parse();
    }

    auto get_ast() const noexcept -> std::shared_ptr<AST::ASTNode>
    { return m_ast_root; }

protected:
    void parse();
    auto statement() -> std::shared_ptr<AST::StatementNode>;
    auto add_sub() -> std::shared_ptr<AST::ASTNode>;
    auto add_sub_tail(std::shared_ptr<AST::ASTNode> lhs) -> std::shared_ptr<AST::ASTNode>;
    auto mul_div() -> std::shared_ptr<AST::ASTNode>;
    auto mul_div_tail(std::shared_ptr<AST::ASTNode> lhs) -> std::shared_ptr<AST::ASTNode>;
    auto operand() -> std::shared_ptr<AST::IntegerNode>;
    void advance_token(std::size_t count = 1);
    auto consume_token(std::function<bool(Token const&)> predicate) -> Token;
}; // class Parser

} // namespace simple_calc

#endif // PARSER_HXX

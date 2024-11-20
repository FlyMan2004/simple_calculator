#include "Parser.hxx"

namespace simple_calc
{

void Parser::parse()
{
    this->m_state.push(State::statement);
    this->m_ast_root = statement();
}

auto Parser::statement() -> std::shared_ptr<AST::StatementNode>
{
    if (this->m_state.top() != State::statement) [[unlikely]]
        throw std::runtime_error("Unexpected state");

    std::shared_ptr<AST::ASTNode> expr;
    this->m_state.pop();
    if (this->m_input_tokens.front().kind == Token::Kind::end_of_statement) [[unlikely]] {
        expr = nullptr;
    } else [[likely]] {
        this->m_input_tokens = this->m_input_tokens.subspan(0, this->m_input_tokens.size() - 1);
        this->m_state.push(State::add_sub);
        expr = this->add_sub();
    }
    auto const end_of_stmt = this->consume_token([](Token const& token) -> bool {
        return token.kind == Token::Kind::end_of_statement;
    });
    this->advance_token();

    return std::make_shared<AST::StatementNode>(std::move(expr));
}

auto Parser::add_sub() -> std::shared_ptr<AST::ASTNode>
{
    if (this->m_state.top() != State::add_sub) [[unlikely]]
        throw std::runtime_error("Unexpected state");

    this->m_state.pop();
    this->m_state.push(State::add_sub_tail);
    this->m_state.push(State::mul_div);

    auto lhs = this->mul_div();
    auto ast = this->add_sub_tail(std::move(lhs));
    
    return ast;
}

auto Parser::add_sub_tail(std::shared_ptr<AST::ASTNode> lhs) -> std::shared_ptr<AST::ASTNode>
{
    if (this->m_state.top() != State::add_sub_tail) [[unlikely]]
        throw std::runtime_error("Unexpected state");

    this->m_state.pop();
    if (this->m_input_tokens.empty())
    {
        return lhs;
    }

    auto const op = this->consume_token([](Token const& token) -> bool {
        return token.kind == Token::Kind::op;
    }).value.op;

    if (op != Op::addition && op != Op::subtraction)
    {
        return lhs;
    }

    this->advance_token();
    this->m_state.push(State::add_sub_tail);
    this->m_state.push(State::mul_div);

    auto rhs = this->mul_div();
    auto ast = std::make_shared<AST::BinaryOperatorNode>(op, std::move(lhs), std::move(rhs));

    return add_sub_tail(std::move(ast));
}

auto Parser::mul_div() -> std::shared_ptr<AST::ASTNode>
{
    if (this->m_state.top() != State::mul_div) [[unlikely]]
        throw std::runtime_error("Unexpected state");

    this->m_state.pop();
    this->m_state.push(State::mul_div_tail);
    this->m_state.push(State::operand);

    auto lhs = this->operand();
    auto ast = this->mul_div_tail(std::move(lhs));

    return ast;
}

auto Parser::mul_div_tail(std::shared_ptr<AST::ASTNode> lhs) -> std::shared_ptr<AST::ASTNode>
{
    if (this->m_state.top() != State::mul_div_tail) [[unlikely]]
        throw std::runtime_error("Unexpected state");

    this->m_state.pop();
    if (this->m_input_tokens.empty())
    {
        return lhs;
    }

    auto const op = this->consume_token([](Token const& token) -> bool {
        return token.kind == Token::Kind::op;
    }).value.op;

    if (op != Op::multiplication && op != Op::division)
    {
        return lhs;
    }

    this->advance_token();
    this->m_state.push(State::mul_div_tail);
    this->m_state.push(State::operand);

    auto rhs = this->operand();
    auto ast = std::make_shared<AST::BinaryOperatorNode>(op, std::move(lhs), std::move(rhs));

    return this->mul_div_tail(std::move(ast));
}

auto Parser::operand() -> std::shared_ptr<AST::IntegerNode>
{
    if (this->m_state.top() != State::operand) [[unlikely]]
        throw std::runtime_error("Unexpected state");

    this->m_state.pop();
    auto const operand = this->consume_token([](Token const& token) -> bool {
        return token.kind == Token::Kind::integer;
    }).value.integer;
    this->advance_token();

    return std::make_shared<AST::IntegerNode>(operand);
}

void Parser::advance_token(std::size_t count)
{
    this->m_input_tokens = this->m_input_tokens.subspan(count);
}

auto Parser::consume_token(std::function<bool(Token const&)> predicate) -> Token
{
    auto const token = this->m_input_tokens.front();
    if (!predicate(token)) [[unlikely]]
        throw std::runtime_error("Unexpected token");

    return token;
}

} // namespace simple_calc

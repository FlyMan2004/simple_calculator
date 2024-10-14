#include <cstdint>
#include <stack>
#include <array>
#include <span>
#include <functional>

#include "expr_eval.hxx"
#include "op.hxx"
#include "utility.hxx"

namespace simple_calc
{

namespace Impl
{

// A "LL(1) parser" using stack to parse the expression and evaluate it.
struct Parser {
public:
    enum class State {
        add_sub,
        add_sub_tail,
        mul_div,
        mul_div_tail,
        operand,
    };
    std::stack<State> state;
    std::span<Token const> input_tokens;
    using EvalResult = std::int32_t;

public:
    fn add_sub(std::optional<EvalResult> evaluated_value = std::nullopt) -> EvalResult;
    fn add_sub_tail(std::optional<EvalResult> evaluated_value = std::nullopt) -> std::optional<EvalResult>;
    fn mul_div(std::optional<EvalResult> evaluated_value = std::nullopt) -> EvalResult;
    fn mul_div_tail(std::optional<EvalResult> evaluated_value = std::nullopt) -> std::optional<EvalResult>;
    fn operand(std::optional<EvalResult> evaluated_value = std::nullopt) -> EvalResult;
    fn next_token() -> void;
    fn consume_token(std::function<bool(Token const&)> predicate) -> Token;
    /*!
     * @param op never be `Op::invalid`
     * @param operands extra operands are considered as an error.
     */
    static fn evaluate(Op op, std::span<std::int32_t const> operands) -> std::int32_t;
};

#define TODO throw std::runtime_error("TODO")

fn Parser::add_sub(std::optional<EvalResult> evaluated_value [[maybe_unused]]) -> EvalResult
{
    if (state.top() != State::add_sub) 
        throw std::runtime_error("Unexpected state");
    
    state.pop();
    state.push(State::add_sub_tail);
    state.push(State::mul_div);
    let const result_mul_div = this->mul_div();
    let const result_expr_tail = this->add_sub_tail(result_mul_div);
    return result_expr_tail.value_or(result_mul_div);
}
fn Parser::add_sub_tail(std::optional<EvalResult> evaluated_value) -> std::optional<EvalResult>
{
    if (state.top() != State::add_sub_tail) 
        throw std::runtime_error("Unexpected state");

    this->state.pop();
    if (this->input_tokens.empty()) {
        return evaluated_value;
    }
    let const op = this->consume_token([](Token const& token) -> bool {
        return token.kind == Token::Kind::op;
    });
    if (op.value.op != Op::addition && op.value.op != Op::subtraction) {
        return std::nullopt;
    }
    
    this->next_token();
    this->state.push(State::add_sub_tail);
    this->state.push(State::operand);
    let const result_operand = this->operand();
    let const result_eval = Parser::evaluate(op.value.op, std::array{evaluated_value.value(), result_operand});
    return this->add_sub_tail(result_eval);
}
fn Parser::mul_div(std::optional<EvalResult> evaluated_value [[maybe_unused]]) -> EvalResult
{
    if (state.top() != State::mul_div) 
        throw std::runtime_error("Unexpected state");
    
    /// Replace the state with others.
    state.pop();
    state.push(State::mul_div_tail);
    state.push(State::operand);
    let const result_operand = this->operand();
    let const result_mul_div_tail = this->mul_div_tail(result_operand);
    return result_mul_div_tail.value_or(result_operand);
}
fn Parser::mul_div_tail(std::optional<EvalResult> evaluated_value) -> std::optional<EvalResult>
{
    if (state.top() != State::mul_div_tail) 
        throw std::runtime_error("Unexpected state");

    this->state.pop();
    if (this->input_tokens.empty()) {
        return evaluated_value;
    }
    let const op = this->consume_token([](Token const& token) -> bool {
        return token.kind == Token::Kind::op;
    });
    if (op.value.op != Op::multiplication && op.value.op != Op::division) {
        return std::nullopt;
    }
    
    this->next_token();
    this->state.push(State::mul_div_tail);
    this->state.push(State::operand);
    let const result_operand = this->operand();
    let const result_eval = Parser::evaluate(op.value.op, std::array{evaluated_value.value(), result_operand});
    return this->mul_div_tail(result_eval);
}
fn Parser::operand(std::optional<EvalResult> evaluated_value [[maybe_unused]]) -> EvalResult
{
    if (state.top() != State::operand) 
        throw std::runtime_error("Unexpected state");
    
    this->state.pop();
    let const operand = this->consume_token([](Token const& token) -> bool {
        return token.kind == Token::Kind::integer;
    });
    this->next_token();
    return operand.value.integer;
}
fn Parser::next_token() -> void
{
    this->input_tokens = this->input_tokens.subspan(1);
}
fn Parser::consume_token(std::function<bool(Token const&)> predicate) -> Token
{
    if (input_tokens.empty()) 
        throw GrammarError("Unexpected end of input");
    
    let const token = input_tokens.front();
    if (!predicate(token)) 
        throw GrammarError("Unexpected token");
    
    return token;
}
fn Parser::evaluate(Op op, std::span<std::int32_t const> operands) -> std::int32_t
{
    [[assume(op != Op::invalid)]];
    switch (op) {
        using enum Op;
    case addition:
        PRE_COND(operands.size() == 2);
        return operands[0] + operands[1];
    case subtraction:
        PRE_COND(operands.size() == 2);
        return operands[0] - operands[1];
    case multiplication:
        PRE_COND(operands.size() == 2);
        return operands[0] * operands[1];
    case division:
        PRE_COND(operands.size() == 2);
        if (operands[1] == 0)
            throw RangeError("Divided by zero");
        return operands[0] / operands[1];
    case invalid:
        unreachable();
    }
    unreachable();
}

#undef TODO

} // namespace Impl

fn expr_eval(std::span<const Token> expr_tokens) noexcept -> std::expected<std::int32_t, GrammarError>
{
    using namespace Impl;
    Parser parser;
    parser.state.push(Parser::State::add_sub);
    parser.input_tokens = expr_tokens;
    try {
        let const result = parser.add_sub();
        return result;
    } catch (GrammarError const& e) {
        return std::unexpected(e);
    }
}

} // namespace simple_calc

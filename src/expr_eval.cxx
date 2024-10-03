#include <expected>
#include <span>
#include <optional>

#include "expr_eval.hxx"

namespace simple_calc {

static fn consume(Token const& token, Token::Kind const expected_kind) noexcept -> std::optional<Token::Value>
{
    return token.kind == expected_kind ? std::optional{token.value} : std::nullopt;
}

namespace Impl {

static fn expr_eval(std::span<const Token> expr_tokens) -> std::int32_t
{
    PRE_COND(!expr_tokens.empty());
    if (expr_tokens.size() == 1) {
        let const integer_opt = consume(*expr_tokens.cbegin(), Token::Kind::integer);
        if (integer_opt.has_value())
            return integer_opt->integer;
        else
            throw GrammarError{ std::format(
                "Unexpected token occurs: {}", 
                Token::kind_to_str(expr_tokens.cbegin()->kind)
            )};
    }
    let const eval = expr_eval({expr_tokens.cbegin(), expr_tokens.cend() - 2});
    let const op_opt = consume(*(expr_tokens.cend() - 2), Token::Kind::op);
    let const integer_opt = consume(*(expr_tokens.cend() - 1), Token::Kind::integer);

    if (!op_opt.has_value())
        throw GrammarError{ std::format(
            "Unexpected token occurs: {}", 
            Token::kind_to_str((expr_tokens.cend() - 2)->kind)
        )};
    if (!integer_opt.has_value())
        throw GrammarError{ std::format(
            "Unexpected token occurs: {}", 
            Token::kind_to_str((expr_tokens.cend() - 1)->kind)
        )};
    std::int32_t result = eval;
    switch (op_opt->op) {
        using enum Op;
    case binary_plus:
        result += integer_opt->integer;
        break;
    case binary_minus:
        result -= integer_opt->integer;
        break;
    case invalid:
        throw GrammarError{ std::format(
            "Invalid token occurs: {}", 
            op_to_str(op_opt->op)
        )};
    }
    return result;
}

}

fn expr_eval(std::span<const Token> expr_tokens) noexcept -> std::expected<std::int32_t, GrammarError>
{
    /// Use `__try` and `__catch` instead to support `-fno-exceptions`
    __try {
        return Impl::expr_eval(expr_tokens);
    } __catch (const GrammarError& err) {
        return std::unexpected{err};
    }
}

}
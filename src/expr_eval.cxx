#include <span>
#include <optional>

#include "expr_eval.hxx"

namespace simple_calc {

static fn consume(Token const& token, Token::Kind const expected_kind) -> std::optional<Token::Value>
{
    return token.kind == expected_kind ? std::optional{token.value} : std::nullopt;
}

fn expr_eval(std::span<const Token> expr_tokens) -> std::expected<std::int32_t, GrammarError>
{
    PRECOND(!expr_tokens.empty());
    if (expr_tokens.size() == 1) {
        let const integer_opt = consume(*expr_tokens.cbegin(), Token::Kind::integer);
        if (integer_opt.has_value())
            return integer_opt->integer;
        else
            return std::unexpected(GrammarError{
                std::format("Unexpected token occurs: {}", Token::kind_to_str(expr_tokens.cbegin()->kind))
            });
    }

    let const eval = expr_eval({expr_tokens.cbegin(), expr_tokens.cend() - 2});
    let const op_opt = consume(*(expr_tokens.cend() - 2), Token::Kind::op);
    let const integer_opt = consume(*(expr_tokens.cend() - 1), Token::Kind::integer);
    std::int32_t result = 0;

    if (!eval.has_value())
        return eval;
    if (!op_opt.has_value())
        return std::unexpected(GrammarError{
            std::format("Unexpected token occurs: {}", Token::kind_to_str((expr_tokens.cend() - 2)->kind))
        });
    if (!integer_opt.has_value())
        return std::unexpected(GrammarError{
            std::format("Unexpected token occurs: {}", Token::kind_to_str((expr_tokens.cend() - 1)->kind))
        });

    result += eval.value();
    switch (op_opt->op) {
        using enum Op;
    case binary_plus:
        result += integer_opt->integer;
        break;
    case binary_minus:
        result -= integer_opt->integer;
        break;
    case invalid:
        return std::unexpected(GrammarError{
            std::format("Invalid token occurs: {}", op_to_str(op_opt->op))
        });
    }

    return result;
}

}
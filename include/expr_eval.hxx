#pragma once
#include <stdexcept>
#ifdef SIMPLE_CALC

#ifndef EXPR_EVAL_HXX
#define EXPR_EVAL_HXX

#include <cstdint>
#include <string>
#include <span>
#include <expected>
#include <exception>

#include "token.hxx"
#include "utility.hxx"

namespace simple_calc {

struct GrammarError : std::exception {
    std::string reason_str;
    GrammarError(std::string reason) noexcept
        : reason_str(std::move(reason))
    {}
    virtual ~GrammarError() override = default;
    virtual fn what() const noexcept -> char const* override { return reason_str.c_str(); }
};
struct RangeError : std::range_error {
    using std::range_error::range_error;
};

extern fn expr_eval(std::span<const Token> expr_tokens) noexcept -> std::expected<std::int32_t, GrammarError>;

}


#endif /* EXPR_EVAL_HXX */

#endif /* SIMPLE_CALC */

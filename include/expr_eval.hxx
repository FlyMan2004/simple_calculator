#pragma once
#ifdef SIMPLE_CALC

#ifndef EXPR_EVAL_HXX
#define EXPR_EVAL_HXX

#include <cstdint>
#include <string>
#include <span>
#include <expected>

#include "token.hxx"
#include "utility.hxx"

namespace simple_calc {

struct GrammarError {
    std::string error_str;
};

extern fn expr_eval(std::span<const Token> expr_tokens) -> std::expected<std::int32_t, GrammarError>;

}


#endif

#endif
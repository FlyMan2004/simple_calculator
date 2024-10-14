#pragma once
#ifdef SIMPLE_CALC

#ifndef OP_CXX
#define OP_CXX

#include <cstdint>
#include <limits>
#include <string_view>
#include "utility.hxx"

namespace simple_calc {

/*!
 * `Op` is short of `Operator`. But it should be produced during **parsing**.
 * E.g. 
 * - In C++, `-` can be used as both binary minus operator and unary minus operator.
 * - `>>` can be right shift operator or two `>` parentheses which denote the end of template arguments like `std::vector<std::vector<int>>`.
 */
enum class Op : std::uint32_t {
    invalid = std::numeric_limits<std::uint32_t>::max(),
    addition = (0x00 << 24) + 0x000000,
    subtraction,
    multiplication = (0x01 << 24) + 0x000000,
    division,
};
[[deprecated("String may be not clear to tell which is selected")]]
constexpr inline fn str_to_op(const std::string_view str) -> Op
{
    Op op = Op::invalid;
    if (str == "+")
        op = Op::addition;
    if (str == "-")
        op = Op::subtraction;
    if (str == "*")
        op = Op::multiplication;
    if (str == "/")
        op = Op::division;
    return op;
}
constexpr inline fn op_to_str(const Op op) -> std::string_view
{
    std::string_view str;
    switch (op) {
    case Op::invalid:
        str = "${invalid}";
        break;
    case Op::addition:
        str = "+";
        break;
    case Op::subtraction:
        str = "-";
        break;
    case Op::multiplication:
        str = "*";
        break;
    case Op::division:
        str = "/";
        break;
    }
    return str;
}

}

#endif

#endif

#pragma once
#ifdef SIMPLE_CALC

#ifndef OP_CXX
#define OP_CXX

#include <cstdint>
#include <string_view>
#include "utility.hxx"

namespace simple_calc {
enum class Op : std::uint32_t {
    invalid = 0,
    binary_plus,
    binary_minus,
};
constexpr inline fn str_to_op(const std::string_view str) -> Op
{
    Op op = Op::invalid;
    if (str == "+")
        op = Op::binary_plus;
    if (str == "-")
        op = Op::binary_minus;
    return op;
}
constexpr inline fn op_to_str(const Op op) -> std::string_view
{
    std::string_view str;
    switch (op) {
    case Op::invalid:
        str = "${invalid}";
        break;
    case Op::binary_plus:
        str = "+";
        break;
    case Op::binary_minus:
        str = "-";
        break;
    }
    return str;
}

}

#endif

#endif
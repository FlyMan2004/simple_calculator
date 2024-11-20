#pragma once

#ifndef AST_NODE_HXX
#define AST_NODE_HXX

#include <memory>
#include <string>
#include <cstdint>
#include <concepts>
#include "utility.hxx"
#include "JSON.hxx"

namespace simple_calc::AST
{

class ASTNode
{
public:
    template <std::integral T>
    using NonMin = T;

    using EvalResult = NonMin<std::int32_t>;

    virtual ~ASTNode() = default;

    // Evaluate the ASTNode and return the result
    virtual auto evaluate() const -> EvalResult = 0;

    // Get JSON data type of the ASTNode
    virtual auto get_available_json_type() const noexcept -> JSON::EnumBaseType = 0;

    // Convert the Node to string
    virtual auto to_string(JSON::Type expect) const noexcept -> std::string = 0;
};

} // namespace simple_calc::AST

#endif // AST_NODE_HXX

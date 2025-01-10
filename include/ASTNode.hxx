#pragma once

#ifndef AST_NODE_HXX
#define AST_NODE_HXX

#include <string>
#include <cstdint>
#include <variant>
#include "JSON.hxx"

namespace simple_calc::AST
{

class ASTNode
{
public:
  ASTNode() noexcept = default;
  ASTNode(const ASTNode &) noexcept = default;
  ASTNode(ASTNode &&) noexcept = default;
  ASTNode &operator=(const ASTNode &) = default;
  ASTNode &operator=(ASTNode &&) noexcept = default;
  using EvalResult = std::variant<std::nullptr_t, bool, std::int64_t, long double, std::string>;

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

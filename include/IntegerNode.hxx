#pragma once

#ifndef INTEGER_NODE_HXX
#define INTEGER_NODE_HXX

#include "ASTNode.hxx"
#include <format>
#include <typeindex>

namespace simple_calc::AST
{

class IntegerNode
  : public ASTNode
{
public:
  using value_type = std::int64_t;

protected:
  value_type m_value;

public:
  IntegerNode() noexcept = delete;
  IntegerNode(const IntegerNode &) noexcept = default;
  IntegerNode(IntegerNode &&) noexcept = default;
  IntegerNode &operator=(const IntegerNode &) noexcept = default;
  IntegerNode &operator=(IntegerNode &&) noexcept = default;
  explicit IntegerNode(value_type value)
    : m_value(value)
  {}

  ~IntegerNode() override = default;

  auto evaluate() const -> EvalResult override
  {
    return m_value;
  }

  auto get_available_json_type() const noexcept -> JSON::EnumBaseType override
  {
    return JSON::Type::number | JSON::Type::object;
  }

  auto to_string(JSON::Type expect) const noexcept -> std::string override
  {
    std::string result;
    switch (expect)
    {
      using enum JSON::Type;
    case number:
      result = std::format("{}", m_value);
      break;
    default:
      [[fallthrough]];
    case object:
      result = std::format(
        "{{\n"
        "  \"type\": \"{}\",\n"
        "  \"value\": {}\n"
        "}}",
        std::type_index(typeid(*this)).name(),
        m_value
      );
      break;
    }
    return result;
  }
}; // class IntegerNode

} // namespace simple_calc::AST

#endif // INTEGER_NODE_HXX

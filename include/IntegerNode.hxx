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
    using value_type = std::int32_t;

protected:
    value_type m_value;

public:
    explicit IntegerNode(value_type value)
        : m_value(value)
    {}

    virtual ~IntegerNode() = default;

    virtual auto evaluate() const -> EvalResult override
    {
        return m_value;
    }

    virtual auto get_available_json_type() const noexcept -> JSON::EnumBaseType override
    {
        return JSON::Type::number | JSON::Type::object;
    }

    virtual auto to_string(JSON::Type expect) const noexcept -> std::string override
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

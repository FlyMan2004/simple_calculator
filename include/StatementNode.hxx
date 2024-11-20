#pragma once

#ifndef STATEMENT_NODE_HXX
#define STATEMENT_NODE_HXX

#include "ASTNode.hxx"
#include <typeindex>

namespace simple_calc::AST
{

class StatementNode
    : public ASTNode
{
protected:
    std::shared_ptr<ASTNode> m_expr;
public:
    explicit StatementNode(std::shared_ptr<ASTNode> expr) noexcept
        : m_expr(expr)
    {}
    virtual ~StatementNode() = default;

    virtual auto evaluate() const -> EvalResult override
    {
        return m_expr == nullptr ? std::numeric_limits<EvalResult>::min() : m_expr->evaluate();
    }

    virtual auto get_available_json_type() const noexcept -> JSON::EnumBaseType override
    {
        return JSON::Type::object;
    }

    virtual auto to_string(JSON::Type expect) const noexcept -> std::string override
    {
        std::string result;
        switch (expect)
        {
            using enum JSON::Type;
        default:
            [[fallthrough]];
        case object:
            result = std::format(
                "{{\n"
                "  \"type\": \"{}\",\n"
                "  \"value\": {}\n"
                "}}",
                std::type_index(typeid(*this)).name(),
                m_expr->to_string(JSON::Type::object)
            );
            break;
        }
        return result;
    }
}; // class StatementNode

} // namespace simple_calc::AST

#endif // STATEMENT_NODE_HXX

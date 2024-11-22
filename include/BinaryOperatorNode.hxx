#pragma once

#ifndef BINARY_OPERATOR_NODE_HXX
#define BINARY_OPERATOR_NODE_HXX

#include <typeindex>
#include "ASTNode.hxx"
#include "op.hxx"

namespace simple_calc::AST
{

class BinaryOperatorNode
    : public ASTNode
{
protected:
    simple_calc::Op m_op;
    std::shared_ptr<ASTNode> m_lhs;
    std::shared_ptr<ASTNode> m_rhs;

public:
    BinaryOperatorNode(
        simple_calc::Op op,
        std::shared_ptr<ASTNode> lhs,
        std::shared_ptr<ASTNode> rhs
    )
        : m_op(op)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {}

    virtual ~BinaryOperatorNode() = default;

    virtual auto evaluate() const -> EvalResult override
    {
        EvalResult result;
        if (!m_lhs || !m_rhs)
            throw std::logic_error("Invalid operand");
        switch (m_op) {
            using enum simple_calc::Op;
        case addition:
            result = m_lhs->evaluate() + m_rhs->evaluate();
            break;
        case subtraction:
            result = m_lhs->evaluate() - m_rhs->evaluate();
            break;
        case multiplication:
            result = m_lhs->evaluate() * m_rhs->evaluate();
            break;
        case division:
            result = m_lhs->evaluate() / m_rhs->evaluate();
            break;
        default:
            throw std::logic_error("Invalid operator");
        }
        return result;
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
            "  \"type\": \"{}\"\n"
            "  \"op\": \"{}\"\n"
            "  \"lhs\": {}\n"
            "  \"rhs\": {}\n"
            "}}",
            std::type_index(typeid(*this)).name(),
            simple_calc::op_to_str(m_op),
            m_lhs ? m_lhs->to_string(JSON::Type::object) : "null",
            m_rhs ? m_rhs->to_string(JSON::Type::object) : "null"
        );
            break;
        }
        return result;
    }

}; // class BinaryOperatorNode

} // namespace simple_calc::AST

#endif // BINARY_OPERATOR_NODE_HXX

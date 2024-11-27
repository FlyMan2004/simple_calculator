#pragma once

#include "utility.hxx"
#include <stdexcept>
#include <variant>
#ifndef BINARY_OPERATOR_NODE_HXX
#define BINARY_OPERATOR_NODE_HXX

#include <typeindex>
#include <memory>
#include "ASTNode.hxx"
#include "op.hxx"

namespace simple_calc::AST
{

class BinaryOperatorNode
    : public ASTNode
{
private:
    // static auto evaluate_impl(simple_calc::Op op, auto const& lhs, auto const& rhs) -> EvalResult
    struct evaluate_impl
    {
        const simple_calc::Op op;
        struct Plus
        {
            template <typename T_lhs, typename T_rhs>
            static auto operator()(T_lhs const& lhs, T_rhs const& rhs) noexcept(noexcept(lhs + rhs)) -> EvalResult
                requires requires { lhs + rhs; }
            {
                return lhs + rhs;
            }
            template <std::formattable<char> T_lhs, std::formattable<char> T_rhs>
            static auto operator()(T_lhs const& lhs, T_rhs const& rhs) -> EvalResult
                requires (!std::same_as<T_lhs, T_rhs> && (std::same_as<T_lhs, std::string> || std::same_as<T_rhs, std::string>))
            {
                return std::format("{}{}", lhs, rhs);
            }
            static auto operator()(auto const&, auto const&) -> EvalResult
            {
                throw std::logic_error("Invalid operand");
                return EvalResult{};
            }
        } constexpr static plus{};

        struct Minus
        {
            template <typename T_lhs, typename T_rhs>
            static auto operator()(T_lhs const& lhs, T_rhs const& rhs) noexcept(noexcept(lhs - rhs)) -> EvalResult
                requires requires { lhs - rhs; }
            {
                return lhs - rhs;
            }
            static auto operator()(auto const&, auto const&) -> EvalResult
            {
                throw std::logic_error("Invalid operand");
                return EvalResult{};
            }
        } constexpr static minus{};

        struct Multiply
        {
            template <typename T_lhs, typename T_rhs>
            static auto operator()(T_lhs const& lhs, T_rhs const& rhs) -> EvalResult
                requires requires { lhs * rhs; }
            {
                return lhs * rhs;
            }
            static auto operator()(auto const&, auto const&) -> EvalResult
            {
                throw std::logic_error("Invalid operand");
                return EvalResult{};
            }
        } constexpr static multiply{};

        struct Divide
        {
            template <typename T_lhs, typename T_rhs>
            static auto operator()(T_lhs const& lhs, T_rhs const& rhs) noexcept(noexcept(lhs / rhs)) -> EvalResult
                requires requires { lhs / rhs; }
            {
                return lhs / rhs;
            }
            static auto operator()(auto const&, auto const&) -> EvalResult
            {
                throw std::logic_error("Invalid operand");
                return EvalResult{};
            }
        } constexpr static divide{};

        auto operator()(auto const& lhs, auto const& rhs) const && -> EvalResult
        {
            [[assume(op != simple_calc::Op::invalid)]];
            EvalResult result;
            switch (op) {
                using enum simple_calc::Op;
            case addition:
                result = plus(lhs, rhs);
                break;
            case subtraction:
                result = minus(lhs, rhs);
                break;
            case multiplication:
                result = multiply(lhs, rhs);
                break;
            case division:
                result = divide(lhs, rhs);
                break;
            default:
                unreachable();
            };
            return result;
        }

        auto operator()(auto const&, auto const&) const & -> EvalResult = delete("`evaluate_impl` should be used as a temporary object");
    };

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
        , m_lhs(std::move(lhs))
        , m_rhs(std::move(rhs))
    {}

    virtual ~BinaryOperatorNode() = default;

    virtual auto evaluate() const -> EvalResult override
    {
        EvalResult result;
        if (!m_lhs || !m_rhs)
            throw std::logic_error("Invalid operand");
        auto lhs = m_lhs->evaluate();
        auto rhs = m_rhs->evaluate();
        switch (m_op) {
            using enum simple_calc::Op;
        case addition: [[fallthrough]];
        case subtraction: [[fallthrough]];
        case multiplication: [[fallthrough]];
        case division:
            result = std::visit(evaluate_impl{m_op}, lhs, rhs);
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

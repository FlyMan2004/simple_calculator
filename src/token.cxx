#include <memory>
#include "token.hxx"

namespace simple_calc {

Token::~Token()
{
    switch (this->kind) {
        using enum Kind;
    case Kind::invalid:
        std::destroy_at(std::addressof(this->value));
        break;

    #define destroy_of_case(kind) \
        case kind: \
            std::destroy_at(std::addressof(this->value.kind)); \
            break

    destroy_of_case(integer);
    destroy_of_case(op);
    destroy_of_case(end_of_statement);
    destroy_of_case(end_of_file);

    #undef destroy_of_case
    }
}

fn Token::kind_to_str(Kind kind) noexcept -> std::string_view
{
    std::string_view str;
    switch (kind) {
        using enum Kind;
    #define case_to_str(var_name, enum_value)   \
        case enum_value:                        \
            var_name = #enum_value;             \
            break

    case invalid:
        str = "${invalid}";
        break;
    case_to_str(str, end_of_statement);
    case_to_str(str, end_of_file);
    case_to_str(str, integer);
    case_to_str(str, op);

    #undef case_to_str
    }
    return str;
}

}
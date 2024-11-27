#include "literal.hxx"

namespace simple_calc {

Literal::~Literal()
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

    destroy_of_case(nil);
    destroy_of_case(boolean);
    destroy_of_case(integer);
    destroy_of_case(floating_point);
    destroy_of_case(string);

    #undef destroy_of_case
    }
}

fn Literal::kind_to_str(Kind kind) noexcept -> std::string_view
{
    std::string_view str;
    switch (kind) {
        using enum Kind;
    #define case_to_str(var_name, enum_value)   \
        case enum_value:                        \
            (var_name) = #enum_value;             \
            break

    case invalid:
        str = "${invalid}";
        break;
    case_to_str(str, nil);
    case_to_str(str, boolean);
    case_to_str(str, integer);
    case_to_str(str, floating_point);
    case_to_str(str, string);

    #undef case_to_str
    }
    return str;
}



} // namespace simple_calc

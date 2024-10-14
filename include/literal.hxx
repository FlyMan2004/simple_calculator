#pragma once
#ifdef SIMPLE_CALC

#ifndef LITERAL_HXX
#define LITERAL_HXX
// Reserve for further use

#include <cstdint>
#include <string_view>

#include "utility.hxx"
namespace simple_calc{

struct Literal {
    enum class Kind {
        invalid = 0,
        integer,
        floating_point,
        string,
    };
    union Value {
        std::int64_t integer;
        long double floating_point;
        std::string_view string;

        Value() noexcept {}
        ~Value() {}
    };

    Kind kind;
    Value value;

    let static constexpr fmt_template = 
        "\"{}\": {{" "\n"
        "   \"variable_type\": \"{}\"," "\n"
        "   \"value\": \"{}\"" "\n"
        "}}";

    Literal(Kind kind = Kind::invalid, Value value = {}) noexcept
        : kind{ kind }, value{ value }
    {}
    ~Literal();

    /* TODO */
};

}

#endif /* LITERAL_HXX */

#endif /* SIMPLE_CALC */
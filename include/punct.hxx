#pragma once
#ifdef SIMPLE_CALC

#ifndef PUNCT_HXX
#define PUNCT_HXX
// Reserve for further use

#include <cstdint>

#include "utility.hxx"

namespace simple_calc {

struct Punct {
    enum class Kind : std::uint32_t {
        invalid         = 0,
        exclamation     = '!',
        quotation       = '\"',
        hash            = '#',
        dollar          = '$',
        percent         = '%',
        ampersand       = '&',
        single_quote    = '\'',
        left_paren      = '(',
        right_paren     = ')',
        asterisk        = '*',
        plus            = '+',
        comma           = ',',
        minus           = '-',
        dot             = '.',
        slash           = '/',
        colon           = ':',
        semicolon       = ';',
        left_angle      = '<',
        equal           = '=',
        right_angle     = '>',
        question        = '?',
        at              = '@',
        left_bracket    = '[',
        backslash       = '\\',
        right_bracket   = ']',
        caret           = '^',
        underscore      = '_',
        back_quote      = '`',
        right_brace     = '}',
        vertical_bar    = '|',
        left_brace      = '{',
        tilde           = '~',
    } value;

    let static constexpr fmt_template = 
        "\"{}\": {{" "\n"
        "   \"variable_type\": \"{}\"," "\n"
        "}}";

    Punct(Kind kind = Kind::invalid) noexcept
        : value{ kind }
    {}
    Punct(char const ch) noexcept
        : value{ std::ispunct(ch) ? static_cast<Kind>(ch) : Punct::Kind::invalid }
    {}
};

}

#endif

#endif
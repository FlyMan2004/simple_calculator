#pragma once
#ifdef SIMPLE_CALC

#ifndef TOKEN_HXX
#define TOKEN_HXX

#include <cctype>
#include <cstdint>
#include <limits>
#include <iostream>
#include <typeinfo>

#include "utility.hxx"
#include "op.hxx"

namespace simple_calc {

/*!
    `EOS` is short of `EndOfStatement`
*/
struct EndOfStmt { 
    let static constexpr value = ';';
};
struct EndOfFile {
    char static constexpr value = EOF;
};

struct Token {
    enum class Kind : std::uint32_t {
        end_of_statement = std::numeric_limits<std::uint32_t>::max() - 1,
        end_of_file,
        invalid = 0,
        integer,
        op,
    };
    union Value {
        [[no_unique_address]]
        EndOfStmt end_of_statement;
        [[no_unique_address]]
        EndOfFile end_of_file;
        std::int32_t integer;
        Op op;

        Value() noexcept {}
        /*!
         *  Ensure the union type can always be destroy if it's of invalid state.
         */
        ~Value() {}
    };

    Kind kind;
    Value value;

    /*!
        Use following template to generate output with std::format().
        See [std::format - cppreference.com](https://en.cppreference.com/w/cpp/utility/format/format) for doc.
    */
    let static constexpr fmt_template = 
        "\"{}\": {{" "\n"
        "   \"variable_type\": \"{}\"," "\n"
        "   \"kind\": \"{}\"," "\n"
        "   \"value\": \"{}\"" "\n"
        "}}";

    ~Token();

    static fn kind_to_str(Kind kind) noexcept -> std::string_view;

    static fn is_valid_kind(Kind const kind) noexcept -> bool
    {
        switch (kind) {
            using enum Kind;
        case end_of_file:
        case invalid:
            return false;
        default:
            return true;
        }
    }

    fn is_valid() const noexcept -> bool 
    { return is_valid_kind(this->kind); }
};

struct Log_Fn {
    ~Log_Fn() { std::clog << std::endl; }
    static fn operator()(Token const& token) -> void
    {
        static bool is_first_token = true;
        if (!is_first_token) [[likely]] {
            std::clog << ",\n";
        } else [[unlikely]] {
            is_first_token = false;
        }
        std::clog << std::format(
            Token::fmt_template, 
            "token", 
            typeid(token).name(), 
            Token::kind_to_str(token.kind), 
            (
                token.kind == Token::Kind::integer ? std::format("{}", token.value.integer) :
                token.kind == Token::Kind::op ? std::format("{}", op_to_str(token.value.op)) :
                token.kind == Token::Kind::end_of_statement ? std::format("{}", ";") :
                token.kind == Token::Kind::invalid ? std::format("{}", "${invalid}") : std::format("")
            )
        );
    }
};
static inline Log_Fn const log;

}

#endif

#endif
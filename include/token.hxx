#pragma once
#ifdef SIMPLE_CALC

#ifndef TOKEN_HXX
#define TOKEN_HXX

#include <cctype>
#include <cstdint>
#include <limits>
#include <memory>
#include <iostream>
#include <sstream>
#include <typeinfo>

#include "utility.hxx"
#include "op.hxx"

namespace simple_calc {

/*!
    `EOS` is short of `EndOfStatement`
*/
struct EndOfStatement { 
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
        EndOfStatement end_of_statement;
        [[no_unique_address]]
        EndOfFile end_of_file;
        std::int32_t integer;
        Op op;

        Value() noexcept {}
        /*!
            Ensure the union type can always be destroy if it's of invalid state.
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

    ~Token()
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

    static fn get_from_istream(std::istream& input = std::cin) -> Token
    {
        Token tok{ .kind = Kind::invalid, .value = {} };
        std::string tok_str = {};
        let const Advance = [&] { input.get(); };

        /*
            Read token character by character.  
            Possible states for ${ch}:
                - null  ( ch == '\0' )
                - digit ( isdigit(ch) == true )
                - punct ( ispunct(ch) == true && (ch == ';' || ch == '+' || ch == '-') )
                - space ( isspace(ch) == true )
                - invalid ( otherwise )

            State transformation:
                null  \
                space ----> digit: token is a integer
                        |-> punct: token is an operator or EOS
                        |-> space: ignore
                        `-> invalid: invalid token
                digit ----> digit: append digit to ${tok_str}
                        `-> punct, space, invalid: token is completed
                punct ----> ${any}: token is completed
            
            Complete this part below.
        */

        for (char ch = '\0'; input.good(); ) {
            ch = input.peek();
            if (std::isdigit(ch)) {
                switch (tok.kind) {
                case Kind::invalid:
                    tok.kind = Kind::integer;
                    [[fallthrough]];
                case Kind::integer:
                    tok_str.push_back(ch);
                    break;
                default:
                    goto token_end;
                }
                Advance();
                continue;
            }
            if (std::ispunct(ch)) {
                switch (tok.kind) {
                case Kind::invalid:
                    switch (ch) {
                    case EndOfStatement::value:
                        tok.kind = Kind::end_of_statement;
                        tok_str = ";";
                        Advance();
                        goto token_end;
                    default:
                        tok.kind = Kind::op;
                        tok_str.push_back(ch);
                    }
                    break;
                default:
                    goto token_end;
                }
                Advance();
                continue;
            }
            if (std::isspace(ch)) {
                Advance();
                if (tok.kind != Kind::invalid) {
                    goto token_end;
                } else {
                    continue;
                }
            }
            /* if (ch is invalid) */
            goto token_end;
        }
    token_end:

        std::destroy_at(std::addressof(tok.value));
        switch (std::stringstream ss{tok_str}; tok.kind) {
        case Kind::invalid:
            break;
        case Kind::integer:
            /* convert tok_str to integer and assign to tok.value */
            {
                std::int32_t integer;
                ss >> integer;
                std::construct_at(std::addressof(tok.value.integer), std::move(integer));
            }
            break;
        case Kind::op:
            /* convert tok_str to Op and assign to tok.value */
            {
                std::construct_at(std::addressof(tok.value.op), str_to_op(tok_str));
            }
            break;
        case Kind::end_of_statement:
            /* convert tok_str to EOS and assign to tok.value */
            {
                std::construct_at(std::addressof(tok.value.end_of_statement), EndOfStatement{});
            }
            break;
        case Kind::end_of_file:
            {
                std::construct_at(std::addressof(tok.value.end_of_file), EndOfFile{});
            }
            break;
        }

        return tok;
    }

    /* Add other member here if needed... */
    static fn kind_to_str(Kind kind) -> std::string_view
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

    struct Log_Fn {
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
    static const Log_Fn Log;
};

}

#endif

#endif
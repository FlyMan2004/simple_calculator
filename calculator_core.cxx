#include <cctype>
#include <cstdint>
#include <limits>
#include <memory>
#include <iostream>
#include <sstream>
#include <string_view>
#include <typeinfo>
/* Add other headers here... */

#define fn  auto
#define let auto

enum class Op : std::uint32_t {
    invalid = 0,
    binary_plus,
    binary_minus,
};
constexpr fn String_To_Op(const std::string_view str) -> Op
{
    Op op = Op::invalid;
    if (str == "+")
        op = Op::binary_plus;
    if (str == "-")
        op = Op::binary_minus;
    return op;
}
constexpr fn Op_To_String(const Op op) -> std::string_view
{
    std::string_view str;
    switch (op) {
    case Op::invalid:
        str = "${invalid}";
        break;
    case Op::binary_plus:
        str = "+";
        break;
    case Op::binary_minus:
        str = "-";
        break;
    }
    return str;
}
/*!
    `EOS` for short of `end_of_statement` 
*/
struct EOS { 
    let static constexpr value = ';';
};

struct Token {
    enum class Kind : std::uint32_t {
        end_of_statement = std::numeric_limits<std::uint32_t>::max(),
        invalid = 0,
        integer,
        op,
    };
    union Value {
        std::int32_t integer;
        Op op;
        [[no_unique_address]]
        EOS end_of_statement;
        Value() noexcept {}
        /*!
            Ensure the union type can always be destory if it's of invalid state.
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
        case Kind::invalid:
            std::destroy_at(std::addressof(this->value));
            break;
        case Kind::integer:
            std::destroy_at(std::addressof(this->value.integer));
            break;
        case Kind::op:
            std::destroy_at(std::addressof(this->value.op));
            break;
        case Kind::end_of_statement:
            std::destroy_at(std::addressof(this->value.end_of_statement));
            break;
        }
    }

    static fn Get_From_Istream() -> Token
    {
        Token tok{ .kind = Kind::invalid, .value = {} };
        std::string tok_str = {};

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
                        |-> punct, space, invalid: token is completed
                punct ----> ${any}: token is completed
            
            Complete this part below.
        */

        let Advance = [] { std::cin.get(); };
        for (char ch = '\0'; std::cin.good(); Advance()) {
            ch = std::cin.peek();
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
                continue;
            }
            if (std::ispunct(ch)) {
                switch (tok.kind) {
                case Kind::invalid:
                    switch (ch) {
                    case EOS::value:
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
                continue;
            }
            if (std::isspace(ch))
                continue;
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
                std::construct_at(std::addressof(tok.value.op), String_To_Op(tok_str));
            }
            break;
        case Kind::end_of_statement:
            /* convert tok_str to EOS and assign to tok.value */
            {
                std::construct_at(std::addressof(tok.value.end_of_statement), EOS{});
            }
            break;
        }

        return tok;
    }

    static fn Kind_To_String(Kind kind) -> std::string_view
    {
    #define case_to_str(var_str, enum_value)    \
        case enum_value:                        \
            var_str = #enum_value;              \
            break

        std::string_view str;
        switch (kind) {
            using enum Kind;
        case invalid:
            str = "${invalid}";
            break;
        case_to_str(str, end_of_statement);
        case_to_str(str, integer);
        case_to_str(str, op);
        }
        return str;
    #undef case_to_str
    }
    /* Add other member here if needed... */
};

/* Add anything you need here... */

int main()
{
    /* Any code here... */
    for (Token token = Token::Get_From_Istream(); token.kind != Token::Kind::invalid; token = Token::Get_From_Istream()) {
        static bool is_first_token = true;
        if (!is_first_token) [[likely]] {
            std::cout << ",\n";
        } else [[unlikely]] {
            is_first_token = false;
        }
        std::cout << std::format(
            Token::fmt_template, 
            "token", 
            typeid(token).name(), 
            Token::Kind_To_String(token.kind), 
            (
                token.kind == Token::Kind::integer ? std::format("{}", token.value.integer) :
                token.kind == Token::Kind::op ? std::format("{}", Op_To_String(token.value.op)) :
                token.kind == Token::Kind::end_of_statement ? std::format("{}", ";") :
                token.kind == Token::Kind::invalid ? std::format("{}", "${invalid}") : std::format("")
            )
        );
    }
    std::cout << std::endl;
}
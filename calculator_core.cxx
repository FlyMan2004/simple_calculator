#include <cassert>
#include <cctype>
#include <cstdint>
#include <format>
#include <limits>
#include <vector>
#include <string_view>
#include <iostream>
#include <string>

#define fn  auto
#define let auto

namespace simple_calc {

struct Token {
    // clang-format off
    enum class Kind : std::uint8_t {
        merged = std::numeric_limits<std::uint8_t>::max(),
        undetermined = 0,
        punct,
        op,
        integer,
        fp,
    };
    enum class Punct : std::uint8_t {
        unknown     = '\0',
        excl        = '!',
        quot        = '\"',
        num         = '#',
        dollar      = '$',
        percnt      = '%',
        amp         = '&',
        apos        = '\'',
        lparen      = '(',
        rparen      = ')',
        ast         = '*',
        plus        = '+',
        comma       = ',',
        hyphen      = '-',
        period      = '.',
        sol         = '/',
        colon       = ':',
        semi        = ';',
        lt          = '<',
        equals      = '=',
        gt          = '>',
        quest       = '?',
        commat      = '@',
        lsqb        = '[',
        bsol        = '\\',
        rsqb        = ']',
        hat         = '^',
        lowbar      = '_',
        grave       = '`',
        lcub        = '{',
        verbal      = '|',
        rcub        = '}',
        tilde       = '~',
    };
    enum class Op : std::uint32_t {
        unary_plus,
        unary_minus,
        addition,
        subtraction,
        multiplication,
        division,
        remainder,
    };
    // clang-format on
    union Value {
        Punct punct;
        Op op;
        std::uint64_t integer;
        double fp;
    };
    Value value;
    Kind kind;

    [[gnu::always_inline, gnu::const]]
    static constexpr fn Create(Kind kind = Kind::undetermined, Value value = Value{}) noexcept -> Token
    { return Token{ .value = value, .kind = kind }; }

    static constexpr fn To_String(Token const& token) -> std::string
    {
        using namespace std::literals;
        std::string kind_str, value_str = "\"To be implemented\""s;
        switch (token.kind)
        {
            using enum Token::Kind;
        case merged:
            kind_str = "merged"s;
            value_str = "$(merged)"s;
            break;
        case undetermined:
            kind_str = "undetermined"s;
            value_str = "$(undetermined)"s;
            break;
        case punct:
            kind_str = "punct"s;
            value_str = token.value.punct != Token::Punct::unknown ? 
                std::format("\'{}\'", (char)(token.value.punct)) :
                "$(unknown punct)"s;
            break;
        case op:
            kind_str = "operator"s;
            break;
        case integer:
            kind_str = "number"s;
            value_str = std::format("{}", token.value.integer);
            break;
        }

        return std::format(
            "Token = {{ .value = {}, .kind = {} }}",
            value_str, kind_str
        );
    }
};

constexpr fn Get_Tokens(std::string_view const src_str) -> std::vector<Token>
{
    std::vector<Token> tokens;
    Token current_token = Token::Create();

    for (let const ch : src_str)
    {
        [[gnu::const]] 
        let static constexpr Char_To_Digit = 
            [](const char ch) noexcept -> std::uint8_t { return ch - '0'; };
        [[gnu::const]] 
        let static constexpr Char_To_Punct = 
            [](const char ch) noexcept -> Token::Punct 
            {
                return std::ispunct(ch) ? static_cast<Token::Punct>(ch) : Token::Punct::unknown;
            };

        if (std::isdigit(ch))
        {
            switch (current_token.kind)
            {
            case Token::Kind::punct:
                tokens.push_back(std::move(current_token));
                current_token = Token::Create();
                [[fallthrough]];
            case Token::Kind::undetermined:
                current_token.kind = Token::Kind::integer;
                current_token.value.integer = Char_To_Digit(ch);
                break;
            case Token::Kind::integer:
                current_token.value.integer = current_token.value.integer * 10 + Char_To_Digit(ch);
                break;
            default:
                assert(current_token.kind != Token::Kind::merged && current_token.kind != Token::Kind::op);
            }
        }
        if (std::ispunct(ch))
        {
            switch (current_token.kind)
            {
            case Token::Kind::integer:
            case Token::Kind::punct:
                tokens.push_back(std::move(current_token));
                current_token = Token::Create();
                [[fallthrough]];
            case Token::Kind::undetermined:
                current_token.kind = Token::Kind::punct;
                current_token.value.punct = Char_To_Punct(ch);
                break;
            default:
                assert(current_token.kind != Token::Kind::merged && current_token.kind != Token::Kind::op);
            }
        }
    }
    assert(current_token.kind != Token::Kind::merged && current_token.kind != Token::Kind::op);
    if (current_token.kind != Token::Kind::undetermined)
    {
        tokens.push_back(std::move(current_token));
    }

    return tokens;
}

}


int main()
{
    std::string expr;
    std::getline(std::cin, expr);

    using namespace simple_calc;
    let const tokens = Get_Tokens(expr);

    for (let const& token : tokens)
    {
        std::cout << Token::To_String(token) << std::endl;
    }

    return 0;
}

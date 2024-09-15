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

struct Token {
    enum class Kind : std::uint8_t {
        merged = std::numeric_limits<std::uint8_t>::max(),
        undetermined = 0,
        punct,
        op,
        number,
    };
    enum class Punct : std::uint8_t {
        unknown = '\0',
        dot = '.',
        plus = '+',
        minus = '-',
        product = '*',
        divide = '/',
    };
    union Value {
        Punct punct;
        std::uint64_t number;
    };
    Value value;
    Kind kind;

    [[gnu::always_inline, gnu::const]]
    static constexpr fn Create(Kind kind = Kind::undetermined, Value value = Value{}) noexcept -> Token
    { return Token{ .value = value, .kind = kind }; }
};

constexpr fn Get_Tokens(const std::string_view src_str) -> std::vector<Token>
{
    std::vector<Token> tokens;
    Token current_token = Token::Create();

    for (let const ch : src_str)
    {
        [[gnu::const]] let static constexpr Char_To_Digit = 
            [](const char ch) noexcept -> std::uint8_t { return ch - '0'; };
        [[gnu::const]] let static constexpr Char_To_Punct = 
            [](const char ch) noexcept -> Token::Punct 
            {
                Token::Punct op;
                switch (ch)
                {
                case '.': 
                    op = Token::Punct::dot; break;
                case '+': 
                    op = Token::Punct::plus; break;
                case '-': 
                    op = Token::Punct::minus; break;
                case '*': 
                    op = Token::Punct::product; break;
                case '/': 
                    op = Token::Punct::divide; break;
                default:
                    op = Token::Punct::unknown; break;
                }
                return op;
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
                current_token.kind = Token::Kind::number;
                current_token.value.number = Char_To_Digit(ch);
                break;
            case Token::Kind::number:
                current_token.value.number = current_token.value.number * 10 + Char_To_Digit(ch);
                break;
            default:
                assert(current_token.kind != Token::Kind::merged && current_token.kind != Token::Kind::op);
            }
        }
        if (std::ispunct(ch))
        {
            switch (current_token.kind)
            {
            case Token::Kind::number:
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

int main()
{
    std::string expr;
    std::getline(std::cin, expr);

    let const tokens = Get_Tokens(expr);
    let constexpr Token_To_String = 
        [](const Token& token) noexcept -> std::string
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
            case number:
                kind_str = "number"s;
                value_str = std::format("{}", token.value.number);
                break;
            }

            return std::format(
                "Token = {{ .value = {}, .kind = {} }}",
                value_str, kind_str
            );
        };

    for (let const& token : tokens)
    {
        std::cout << Token_To_String(token) << std::endl;
    }

    return 0;
}
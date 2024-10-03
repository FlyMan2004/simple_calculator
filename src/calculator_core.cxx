#include <iostream>
#include <vector>
/* Add other headers here... */

#include "token.hxx"
#include "expr_eval.hxx"

fn Get_Statement() -> std::vector<simple_calc::Token>
{
    using namespace simple_calc;
    std::vector<Token> tokens;
    do {
        tokens.push_back(Token::get_from_istream());
        Token::Log(tokens.back());
    } while (tokens.back().kind != Token::Kind::end_of_statement);
    return tokens;
}

int main()
{
    /* Any code here... */
    using namespace simple_calc;
    let Is_Null_Statement = 
        [](std::vector<Token> const& statements) -> bool
        {
            return statements.size() == 1 && statements.front().kind == Token::Kind::end_of_statement;
        };
    for (let statement = Get_Statement(); !Is_Null_Statement(statement); statement = Get_Statement()) {
        let const result = expr_eval({statement.cbegin(), statement.cend() - 1});
        if (!result.has_value()) {
            std::cerr << result.error().error_str << std::endl;
            break;
        }
        std::cout << std::format("{}\n", result.value());
    }
}
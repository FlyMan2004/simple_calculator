#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
/* Add other headers here... */

#include "token.hxx"
#include "expr_eval.hxx"
#include "utility.hxx"

fn get_statement() -> std::vector<simple_calc::Token>
{
    using namespace simple_calc;
    std::vector<Token> tokens;
    do {
        tokens.push_back(Token::get_from_istream());
        log(tokens.back());
    } while (tokens.back().is_valid() && tokens.back().kind != Token::Kind::end_of_statement);
    POST_COND(!tokens.empty());
    return tokens;
}

int main()
{
    /* Any code here... */
    using namespace simple_calc;
    let is_null_statement = 
        [](std::vector<Token> const& statement) -> bool
        {
            let const& tokens = statement;
            return tokens.size() == 1 && tokens.front().kind == Token::Kind::end_of_statement;
        };
    let is_invalid_statement = 
        [](std::vector<Token> const& statement) -> bool
        {
            let const& tokens = statement;
            return statement.empty() || std::ranges::any_of(tokens, [](Token const& tok) -> bool { return !tok.is_valid(); });
        };
    for (let statement = get_statement(); !is_null_statement(statement); statement = get_statement()) {
        if (is_invalid_statement(statement)) {
            std::cerr << "An error is omitted. Exit..." << std::endl;
            return EXIT_FAILURE;
        }
        let const result = expr_eval({statement.cbegin(), statement.cend() - 1});
        if (!result.has_value()) {
            std::cerr << result.error().what() << std::endl;
            break;
        }
        std::cout << std::format("{}\n", result.value());
    }
}
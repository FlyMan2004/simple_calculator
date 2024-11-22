#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
/* Add other headers here... */

#include "token.hxx"
#include "lexer.hxx"
#include "Parser.hxx"
#include "utility.hxx"

fn get_stmt(simple_calc::Lexer &lexer) -> std::vector<simple_calc::Token>
{
    using namespace simple_calc;
    std::vector<Token> tokens;
    do {
        tokens.push_back(lexer.get_token());
        log(tokens.back());
    } while (tokens.back().is_valid());
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

    Lexer lexer;
    for (let statement = get_stmt(lexer); !is_null_statement(statement); statement = get_stmt(lexer)) {
        if (is_invalid_statement(statement)) {
            std::cerr << "An error is omitted. Exit..." << std::endl;
            return EXIT_FAILURE;
        }
        let parser = Parser({statement.cbegin(), statement.cend()});
        let const result = parser.get_ast()->evaluate();
        let const json = parser.get_ast()->to_string(JSON::Type::object);
        std::clog << json << std::endl;
        std::cout << std::format("{}\n", result);
    }
}

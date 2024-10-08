#include <utility>
#include <sstream>
#include "lexer.hxx"
#include "token.hxx"

namespace simple_calc {

Lexer::InputSource::InputSource(std::filesystem::path file_path)
    : path{ std::move(file_path) }
{
    if (this->is_from_stdin()) {
        std::construct_at(std::addressof(this->m_stdin), std::cin);
    } else {
        this->m_file.open(this->path);
    }
}

Lexer::InputSource::~InputSource()
{
    if (this->path.empty()) {
        std::destroy_at(std::addressof(this->m_stdin));
    } else {
        this->m_file.close();
    }
}

fn Lexer::InputSource::file() const -> std::istream const&
{
    return this->is_from_stdin() ? 
        std::as_const(this->m_stdin.get()) : 
        this->m_file;
}

fn Lexer::InputSource::file() -> std::istream&
{ return const_cast<std::istream&>(std::as_const(*this).file()); }

fn Lexer::InputSource::is_from_stdin() const noexcept -> bool
{ return this->path.empty(); }

fn Lexer::peek() -> char
{ return this->m_source.file().peek(); }

fn Lexer::advance() -> void
{
    let ch = this->m_source.file().get();
    this->m_column_pos++;
    if (ch == '\n') {
        this->m_line_num++;
        this->m_column_pos = 0;
    }
}

fn Lexer::get_token() -> Token
{
    Token tok{ .kind = Token::Kind::invalid, .value = {} };
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
                    `-> punct, space, invalid: token is completed
            punct ----> ${any}: token is completed
    */
    for (char ch = '\0'; this->m_source.file().good(); ) {
        ch = this->peek();
        if (std::isdigit(ch)) {
            switch (tok.kind) {
                using enum Token::Kind;
            case invalid:
                tok.kind = integer;
                [[fallthrough]];
            case integer:
                tok_str.push_back(ch);
                break;
            default:
                goto token_end;
            }
            this->advance();
            continue;
        }
        if (std::ispunct(ch)) {
            switch (tok.kind) {
                using enum Token::Kind;
            case invalid:
                switch (ch) {
                case EndOfStmt::value:
                    tok.kind = end_of_statement;
                    tok_str = ";";
                    this->advance();
                    goto token_end;
                default:
                    tok.kind = op;
                    tok_str.push_back(ch);
                }
                break;
            default:
                goto token_end;
            }
            this->advance();
            continue;
        }
        if (std::isspace(ch)) {
            this->advance();
            if (tok.kind != Token::Kind::invalid) {
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
        using enum Token::Kind;
    case invalid:
        break;
    case integer:
        /* convert tok_str to integer and assign to tok.value */
        {
            std::int32_t integer;
            ss >> integer;
            std::construct_at(std::addressof(tok.value.integer), std::move(integer));
        }
        break;
    case op:
        /* convert tok_str to Op and assign to tok.value */
        {
            std::construct_at(std::addressof(tok.value.op), str_to_op(tok_str));
        }
        break;
    case end_of_statement:
        /* convert tok_str to EOS and assign to tok.value */
        {
            std::construct_at(std::addressof(tok.value.end_of_statement), EndOfStmt{});
        }
        break;
    case end_of_file:
        {
            std::construct_at(std::addressof(tok.value.end_of_file), EndOfFile{});
        }
        break;
    }

    return tok;
}

}
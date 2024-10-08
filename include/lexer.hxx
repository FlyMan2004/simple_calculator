#pragma once
#include <fstream>
#ifdef SIMPLE_CALC

#ifndef LEXER_HXX
#define LEXER_HXX

#include <functional>
#include <filesystem>
#include <cstddef>

#include "utility.hxx"
#include "token.hxx"

namespace simple_calc {

// One lexer per file
class Lexer {
protected:
    /// Source file can be stdin or a file.
    struct InputSource{
        /// Path to the file storing the prog. `stdin` if empty.
        std::filesystem::path /* const? */ path;
        union {
            std::fstream m_file;
            std::reference_wrapper<std::istream> m_stdin;
        };
        InputSource(std::filesystem::path file_path);
        ~InputSource();

        fn is_from_stdin() const noexcept -> bool;
        fn file() const -> std::istream const&;
        fn file() -> std::istream&;
    } m_source;
    std::size_t m_line_num{};
    std::size_t m_column_pos{};
    
public:
    Lexer(std::filesystem::path file_path = {})
        : m_source{ std::move(file_path) }
    {}

    fn peek() -> char;
    fn advance() -> void;
    fn get_token() -> Token;
};

}
#endif

#endif
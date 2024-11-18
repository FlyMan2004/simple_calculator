#pragma once
#ifdef SIMPLE_CALC

#ifndef UTILITY_CXX
#define UTILITY_CXX

#include <iostream>
#include <string_view>
#include <source_location>
#include <stacktrace>
#include <format>

#define fn  auto
#define let auto

namespace simple_calc {

namespace Impl {

[[gnu::always_inline]]
inline fn basic_assert(
    bool const cond, 
    std::string_view const msg,
    std::string_view const failure_kind,
    std::source_location const loc = std::source_location::current(), 
    std::stacktrace const st = std::stacktrace::current()
) noexcept -> void
{
    if (cond) return;
    std::cerr << std::format(
        "{} in: {}({}:{}) `{}`: expect `{}`\n"
        "Stacktrace at this point:\n"
        "{}\n",
        failure_kind, loc.file_name(), loc.line(), loc.column(), loc.function_name(), msg,
        st
    );
    std::terminate();
}

}

#define unreachable __builtin_unreachable

#ifdef DEBUG

#define ASSERT(...) \
    do { \
        bool const expr = __VA_ARGS__; \
        std::string_view const expr_str = #__VA_ARGS__; \
        Impl::basic_assert( \
            expr, \
            expr_str, \
            "Assertion failed" \
        ); \
    } while (0)
#define PRE_COND(...) \
    do { \
        bool const expr = __VA_ARGS__; \
        std::string_view const expr_str = #__VA_ARGS__; \
        Impl::basic_assert( \
        expr, \
        expr_str, \
        "Pre-condition does not meet" \
        ); \
    } while (0)
#define POST_COND(...) \
    do { \
        bool const expr = __VA_ARGS__; \
        std::string_view const expr_str = #__VA_ARGS__; \
        Impl::basic_assert( \
            static_cast<bool>(expr), \
            expr_str, \
            "Post-condition does not meet" \
        ); \
    } while (0)

#else

#define ASSERT(...) \
    do { } while (0)
#define PRE_COND(...) \
    do { } while (0)
#define POST_COND(...) \
    do { } while (0)

#endif

}


#endif

#endif

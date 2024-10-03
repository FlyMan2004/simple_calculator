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

#define ASSERT(...) \
    Impl::basic_assert( \
        static_cast<bool>(__VA_ARGS__), \
        #__VA_ARGS__, \
        "Assertion failed"\
    )
#define PRE_COND(...) \
    Impl::basic_assert( \
        static_cast<bool>(__VA_ARGS__), \
        #__VA_ARGS__, \
        "Pre-condition does not meet" \
    )
#define POST_COND(...) \
    Impl::basic_assert( \
        static_cast<bool>(__VA_ARGS__), \
        #__VA_ARGS__, \
        "Post-condition does not meet" \
    )
}

#endif

#endif
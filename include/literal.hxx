#pragma once
#ifdef SIMPLE_CALC

#ifndef LITERAL_HXX
#define LITERAL_HXX
// Reserve for further use

#include <cstdint>
#include <string_view>

#include "utility.hxx"
namespace simple_calc{

struct Literal {
    enum class Kind : std::uint8_t {
        invalid = 0,
        nil,
        boolean,
        integer,
        floating_point,
        string,
    };
    union Value {
        std::nullptr_t nil;
        bool boolean;
        /// At this point, we simply use the largest type `int64_t` to store the value rather than a `Integer` class
        /// that can store the value of any integer type of any size
        std::int64_t integer;
        /// At this point, we simply use `long double` to store the value rather than a more flexible `FP` class
        long double floating_point;
        /// Just a `std::string_view` to store the reference to that in source code
        std::string_view string;

        Value() noexcept : nil{} {}
        Value(Value const&) noexcept = default;
        Value(Value&&) noexcept = default;
        Value& operator=(Value const&) noexcept = default;
        Value& operator=(Value&&) noexcept = default;
        Value(std::nullptr_t) noexcept : nil{} {}
        Value(bool value) noexcept : boolean{ value } {}
        Value(std::int64_t value) noexcept : integer{ value } {}
        Value(long double value) noexcept : floating_point{ value } {}
        Value(std::string_view value) noexcept : string{ value } {}
        ~Value() {}
    };

    Kind kind;
    Value value;

    let static constexpr fmt_template =
        "{{" "\n"
        "   \"variable_name\": \"{}\"," "\n"
        "   \"variable_type\": \"{}\"," "\n"
        "   \"value\": \"{}\"" "\n"
        "}}";

    Literal() noexcept = delete;
    Literal(const Literal&) noexcept = default;
    Literal(Literal&&) noexcept = default;
    Literal& operator=(const Literal&) noexcept = default;
    Literal& operator=(Literal&&) noexcept = default;

    explicit Literal(Kind kind = Kind::invalid, Value value = {}) noexcept
        : kind{ kind }, value{ std::move(value) }
    {}
    ~Literal();

    static fn kind_to_str(Kind kind) noexcept -> std::string_view;
    static fn is_valid_kind(Kind kind) noexcept -> bool
    { return kind != Kind::invalid; }
}; // struct Literal

} // namespace simple_calc

#endif /* LITERAL_HXX */

#endif /* SIMPLE_CALC */

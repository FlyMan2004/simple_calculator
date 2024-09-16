#include <cinttypes>
#include <random>
#include <print>
#include <cstdio>
#include <cstdint>
#include <format>
#include <array>

#define fn  auto
#define let auto

fn main() -> int
{
    std::uint32_t test_scale;
    
    std::scanf("%" PRIu32, &test_scale);
    std::random_device rd{};
    std::ranlux24_base re{rd()};
    std::uniform_int_distribution<std::uint32_t> dist{0, test_scale};

    let constexpr dictionary = std::array{
        '.', '+', '-', '*', '/'
    };

    for (std::uint32_t i = 0; i < test_scale; ++i)
    {
        static std::string output;
        if (i % (1 << 16) == 0)
            std::println(stderr, "{} has been done.", i);
        let const value = dist(rd);
        output = (value % 4 == 0) ? 
            std::format("{}", dictionary[(value >> 2) % dictionary.size()]) : 
            std::format("{}", value);
        std::print("{} ", output);
    }

    return 0;
}
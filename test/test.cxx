#include <random>
#include <iostream>
#include <print>

int main()
{
    std::random_device rd{};
    std::ranlux24_base engine{rd()};
    std::uniform_int_distribution<std::uint32_t> distri{};
    
    bool two_op, a_nlt_b;
    std::uint32_t n;
    std::uint32_t a, b;
    std::int64_t ans;
    char op;

    std::cin >> two_op >> a_nlt_b >> n;
    for (std::uint32_t i = 0; i < n; ++i)
    {
        using Param = typename std::uniform_int_distribution<std::uint32_t>::param_type;
        op = (two_op ? distri(rd, Param{0, 1}) : 1) ? '+' : '-';
        a = distri(rd);
        b = a_nlt_b ? distri(rd, Param{0, a}) : distri(rd);
        std::println("{} {} {}", a, op, b);
        switch (op) {
        case '+':
            ans = std::int64_t(a) + std::int64_t(b);
            break;
        case '-':
            ans = std::int64_t(a) - std::int64_t(b);
            break;
        default:
            std::terminate();
        }
        std::println(stderr, "{}", ans);
    }
}
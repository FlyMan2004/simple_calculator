#include <iostream>
#include <cstdint>
#include <cassert>

int main()
{
    std::int64_t a, b, n;
    std::cin >> n;
    for (std::int64_t i = 0; i < n; ++i)
    {
        char op;
        std::int64_t ans;
        std::cin >> a >> op >> b;
        assert(op == '+' || op == '-');
        switch (op)
        {
        case '+':
            ans = a + b;
            break;
        case '-':
            ans = a - b;
            break;
        default:
            __builtin_unreachable();
        }
        std::cout << ans << std::endl;
    }
    return 0;
}
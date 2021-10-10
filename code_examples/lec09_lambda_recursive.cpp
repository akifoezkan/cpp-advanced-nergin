#include <iostream>
//#include <format>

int main() {

    const auto factorial = [](int n) {
        const auto factorial_impl = [](int n, const auto& impl_f) -> int {
            return n > 1 ? n * impl_f(n - 1, impl_f) : 1;
        };

        return factorial_impl(n, factorial_impl);
    };

    std::cout << factorial(5) << "\n";
    //std::cout << std::format("{}! = {}\n", 5, factorial(5));

    return 0;
}

#include <iostream>
#include <string_view>
#include <iomanip>
#include <charconv>
#include <system_error>

int main()
{
    for(std::string_view const str : {"15 foo", "bar", "bar 15", "42", "5000000"})
    {
        std::cout << "String " << std::quoted(str) << ". ";

        int result{};

        auto [ptr, ec] {std::from_chars(str.data(), str.data() + str.size(), result)};

        if(ec == std::errc())
        {
            std::cout << "Result " << result << ", ptr -> " << std::quoted(ptr) << "\n";
        }
        else if (ec == std::errc::invalid_argument)
        {
            std::cout << "That is not a number\n";
        }
        else if(ec == std::errc::result_out_of_range)
        {
            std::cout << "The number is larger than int\n";
        }
    }
}

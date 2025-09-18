#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>

int main() {
    std::string input {};
    std::cout << "Please enter a system path: ";
    std::cin >> input;

    std::filesystem::file_time_type last_write_time { std::filesystem::last_write_time(input) };
    std::cout << std::format("The current time is: {}", std::chrono::system_clock::now()) << '\n';
    std::cout << std::format("Last write to '{}': {}", input, last_write_time) << std::endl;

    return EXIT_SUCCESS;
}

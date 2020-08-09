#ifndef PE_MAIN_H
#define PE_MAIN_H

#include <chrono>
#include <iostream>
#include <string>

std::string solution();

int main() {
    auto start_time = std::chrono::steady_clock::now();

    std::string answer = solution();
    std::cout << "Answer: " << answer << std::endl;

    auto end_time = std::chrono::steady_clock::now();
    auto run_time = end_time - start_time;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (run_time).count() << " ms" << std::endl;

    return 0;
}

#endif

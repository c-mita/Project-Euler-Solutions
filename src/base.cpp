#include <iostream>
#include <chrono>

int main() {
    auto startTime = std::chrono::steady_clock::now();

    std::cout << "Answer: " << 42 << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}

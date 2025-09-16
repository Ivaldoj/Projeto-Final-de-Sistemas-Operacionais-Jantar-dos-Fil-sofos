#include <vector>
#include <iostream>
#include "Philosopher.hpp"

int main() {
    std::vector<Philosopher> ph;
    ph.reserve(5);
    for (int i = 0; i < 5; ++i) ph.emplace_back(i);

    std::cout << "=== Dining Philosophers ===\n";
    for (const auto& p : ph) p.show();
    std::cout << "===========================\n";
    return 0;
}
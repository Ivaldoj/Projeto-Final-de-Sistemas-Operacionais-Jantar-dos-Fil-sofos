#include "diningTable.hpp"
#include "philosopher.hpp"
#include <iostream>

DiningTable::DiningTable(int n) {
    philosophers_.reserve(n);
    for (int i = 0; i < n; ++i) {
        philosophers_.emplace_back(i);
    }
}

void DiningTable::showPhilosophers() const {
    std::cout << "=== Dining Table ===\n";
    for (const auto& p : philosophers_) {
        p.show();
    }
    std::cout << "====================\n";
}
#pragma once
#include <vector>
#include "Philosopher.hpp"

class DiningTable {
public:
    explicit DiningTable(int n = 5);

    void showPhilosophers() const;

private:
    std::vector<Philosopher> philosophers_;
};
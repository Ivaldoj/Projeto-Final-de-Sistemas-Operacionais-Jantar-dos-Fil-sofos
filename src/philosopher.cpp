#include "Philosopher.hpp"
#include <iostream>

Philosopher::Philosopher(int id)
    : id_(id), name_("Philosopher " + std::to_string(id)) {}

int Philosopher::id() const {
    return id_;
}

const std::string& Philosopher::name() const {
    return name_;
}

void Philosopher::show() const {
    std::cout << "[NEW] " << name_ << " (id=" << id_ << ")\n";
}

#pragma once
#include <string>

class Philosopher {
public:
    explicit Philosopher(int id);

    int id() const;
    const std::string& name() const;

    // Só para mostrar
    void show() const;

private:
    int id_;
    std::string name_;
};

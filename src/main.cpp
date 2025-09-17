#include <iostream>
#include <vector>
#include <string>

#include "philosopher.hpp"
#include "diningtable.hpp"

enum class SolutionType {
    Monitor = 1,
    Semaphores,
    MutexForks,
    WaiterArbiter
};

// solução com monitor
void run_monitor_solution() {
    const int N = 5;
    DiningTable table(N);
    table.showPhilosophers(); 
}
// solução com semáforos
void run_semaphores_solution() {
    std::cout << "Implementar solução com semáforos por filósofo.\n";
}
// solução com mutex por garfo
void run_mutexforks_solution() {
    std::cout << "Implementar solução com um mutex por garfo.\n";
}
// solução com garçom que limita quem pode comer
void run_waiter_solution() {
    std::cout << "Implementar solução com garçom que limita quem pode comer.\n";
}

// função para executar a solução escolhida
void run_solution(SolutionType t) {
    switch (t) {
        case SolutionType::Monitor:        run_monitor_solution();        break;
        case SolutionType::Semaphores:     run_semaphores_solution();     break;
        case SolutionType::MutexForks:     run_mutexforks_solution();     break;
        case SolutionType::WaiterArbiter:  run_waiter_solution();         break;
    }
}

int main() {
    while (true) {
        std::cout << "\n=== Dining Philosophers — Escolha a Solucao ===\n"
                  << "1) Monitor \n"
                  << "2) Semaforos \n"
                  << "3) Mutex por garfo \n"
                  << "4) Garcom / Arbitro \n"
                  << "0) Sair\n> ";

        int op; 
        if (!(std::cin >> op)) return 0;
        if (op == 0) break;
        if (op < 0 || op > 4) {
            std::cout << "Opcao invalida.\n";
            continue;
        }
        run_solution(static_cast<SolutionType>(op));
    }
    return 0;
}

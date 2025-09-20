#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include "philosopher.hpp"
#include "diningtable.hpp"
#include "mutexmode.hpp"
#include "waitermode.hpp"

enum class SolutionType {
    Monitor = 1,
    Semaphores,
    MutexForks,
    WaiterArbiter
};

// função para executar a solução escolhida
void run_solution(SolutionType t) {
    const int N = 5;
    std::vector<Philosopher> philosophers;
    philosophers.reserve(N);
    for (int i = 0; i < N; i++) philosophers.emplace_back(i);

    switch (t) {
        case SolutionType::Monitor:
            std::cout << "[MODO MONITOR] ainda nao implementado\n";
            break;
        case SolutionType::Semaphores:
            std::cout << "[MODO SEMAFORO] ainda nao implementado\n";
            break;
        case SolutionType::MutexForks:
            runMutexMode(philosophers);
            break;
        case SolutionType::WaiterArbiter:
        runWaiterMode(philosophers);
            break;
    }
}

int main() {
    while (true) {
        std::cout << "\n=== Dining Philosophers. Escolha a Solucao ===\n"
                  << "1) Monitor (mutex + condvar)\n"
                  << "2) Semaforos\n"
                  << "3) Mutex por garfo\n"
                  << "4) Garcom / Arbitro\n"
                  << "0) Sair\n> ";

        int op;
        if (!(std::cin >> op)) return 0;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        if (op == 0) break;
        if (op < 0 || op > 4) {
            std::cout << "Opcao invalida.\n";
            continue;
        }
        run_solution(static_cast<SolutionType>(op));
    }
    return 0;
}

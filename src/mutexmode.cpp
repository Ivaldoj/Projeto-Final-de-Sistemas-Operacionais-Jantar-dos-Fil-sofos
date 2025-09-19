#include "mutexmode.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <numeric>

namespace {
    std::mutex forks[5];         // 5 garfos (um mutex por garfo)
    std::mutex stats_mutex;      // protege estatísticas

    const int ROUNDS = 500;      // número fixo de rodadas por filósofo

    struct PhilosopherStats {
        int meals = 0;
        std::vector<long long> wait_times; // em ms
    };

    std::vector<PhilosopherStats> stats(5);

    void philosopherLife(int id) {
        int left = id;
        int right = (id + 1) % 5;

        for (int count = 0; count < ROUNDS; count++) {
            // marca início da espera
            auto start_wait = std::chrono::high_resolution_clock::now();

            // tenta pegar os garfos
            std::lock(forks[left], forks[right]);
            std::lock_guard<std::mutex> lockLeft(forks[left], std::adopt_lock);
            std::lock_guard<std::mutex> lockRight(forks[right], std::adopt_lock);

            // marca fim da espera
            auto end_wait = std::chrono::high_resolution_clock::now();
            long long wait_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(end_wait - start_wait).count();

            {
                std::lock_guard<std::mutex> lg(stats_mutex);
                stats[id].meals++;
                stats[id].wait_times.push_back(wait_time);
            }

            // simula comendo (curto, só para testar rápido)
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));
        }
    }
}

void runMutexMode(std::vector<Philosopher>& philosophers) {
    std::cout << "[MODO MUTEX] executando...\n";
    srand(static_cast<unsigned>(time(nullptr))); // inicializa seed do rand

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < (int)philosophers.size(); i++) {
        threads.emplace_back(philosopherLife, i);
    }
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    long long total_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // imprime estatísticas
    std::cout << "\n=== RESULTADOS MODO MUTEX ===\n";
    for (int i = 0; i < (int)philosophers.size(); i++) {
        auto& s = stats[i];
        double avg_wait = 0.0;
        if (!s.wait_times.empty()) {
            avg_wait = std::accumulate(s.wait_times.begin(), s.wait_times.end(), 0LL) /
                       (double)s.wait_times.size();
        }
        std::cout << "Filosofo " << i
                  << " -> refeicoes: " << s.meals
                  << ", espera media: " << avg_wait << " ms\n";
    }
    std::cout << "Tempo total da simulacao: " << total_time << " ms\n";
    std::cout << "==============================\n";
}

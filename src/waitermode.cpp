#include "waitermode.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <numeric>

namespace {
    const int N = 5;            // número de filósofos
    const int ROUNDS = 200;     // rodadas por filósofo

    std::mutex mtx;             // controla acesso ao garçom
    std::condition_variable cv; // usado para esperar liberação
    int eating = 0;             // quantos estão comendo agora
    std::mutex stats_mutex;     // protege estatísticas

    struct PhilosopherStats {
        int meals = 0;
        std::vector<long long> wait_times;
    };

    std::vector<PhilosopherStats> stats(N);

    void philosopherLife(int id) {
        for (int count = 0; count < ROUNDS; count++) {
            auto start_wait = std::chrono::high_resolution_clock::now();

            {
                std::unique_lock<std::mutex> lock(mtx);
                // espera até que menos de N-1 estejam comendo
                cv.wait(lock, [] { return eating < N - 1; });
                eating++;
            }

            auto end_wait = std::chrono::high_resolution_clock::now();
            long long wait_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(end_wait - start_wait).count();

            {
                std::lock_guard<std::mutex> lg(stats_mutex);
                stats[id].meals++;
                stats[id].wait_times.push_back(wait_time);
            }

            // simula comer
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));

            {
                std::unique_lock<std::mutex> lock(mtx);
                eating--;
                cv.notify_all(); // acorda todos que estão esperando
            }

            // simula pensar
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));
        }
    }
}

void runWaiterMode(std::vector<Philosopher>& philosophers) {
    std::cout << "[MODO GARCOM] executando...\n";
    srand(static_cast<unsigned>(time(nullptr)));

    auto start = std::chrono::high_resolution_clock::now();

    // cria threads
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
    std::cout << "\n=== RESULTADOS MODO GARCOM ===\n";
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

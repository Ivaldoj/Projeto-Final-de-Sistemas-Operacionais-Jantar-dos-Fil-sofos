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
    std::mutex forks[5];         // um mutex para cada garfo
    std::mutex stats_mutex;      // protege acesso às estatísticas

    const int ROUNDS = 200;      // rodadas por filósofo

    struct PhilosopherStats {
        int meals = 0;                          // refeições feitas
        std::vector<long long> wait_times;      // tempos de espera em ms
    };

    std::vector<PhilosopherStats> stats(5);

    // função que representa a vida do filósofo
    void philosopherLife(int id) {
        int left = id;                // garfo da esquerda
        int right = (id + 1) % 5;     // garfo da direita

        for (int count = 0; count < ROUNDS; count++) {
            // marca início do tempo de espera
            auto start_wait = std::chrono::high_resolution_clock::now();

            // bloqueia os dois garfos de forma atômica
            std::lock(forks[left], forks[right]);
            std::lock_guard<std::mutex> lockLeft(forks[left], std::adopt_lock);
            std::lock_guard<std::mutex> lockRight(forks[right], std::adopt_lock);

            // marca fim da espera
            auto end_wait = std::chrono::high_resolution_clock::now();
            long long wait_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(end_wait - start_wait).count();

            // atualiza estatísticas protegidas por mutex
            {
                std::lock_guard<std::mutex> lg(stats_mutex);
                stats[id].meals++;
                stats[id].wait_times.push_back(wait_time);
            }

            // simula tempo de comer
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));
        }
    }
}

// Função principal do modo Mutex
void runMutexMode(std::vector<Philosopher>& philosophers) {
    std::cout << "[MODO MUTEX] executando...\n";
    srand(static_cast<unsigned>(time(nullptr)));

    auto start = std::chrono::high_resolution_clock::now();

    // cria threads
    std::vector<std::thread> threads;
    for (int i = 0; i < (int)philosophers.size(); i++) {
        threads.emplace_back(philosopherLife, i);
    }

    // aguarda todas terminarem
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

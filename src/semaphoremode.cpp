#include "semaphoremode.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <semaphore.h>
#include <mutex>
#include <numeric>

namespace {
    const int N = 5;
    const int ROUNDS = 200; // rodadas fixas para teste

    sem_t forks[N];        // semáforo para cada garfo
    sem_t room;            // semáforo global (limita N-1 filósofos)
    std::mutex stats_mutex;

    struct PhilosopherStats {
        int meals = 0;
        std::vector<long long> wait_times;
    };

    std::vector<PhilosopherStats> stats(N);

    void philosopherLife(int id) {
        int left = id;
        int right = (id + 1) % N;

        for (int count = 0; count < ROUNDS; count++) {
            auto start_wait = std::chrono::high_resolution_clock::now();

            // entra na "sala" (limite de N-1 filósofos simultâneos)
            sem_wait(&room);

            // pega garfos (ordem pode ser arbitrária, mas consistente)
            sem_wait(&forks[left]);
            sem_wait(&forks[right]);

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

            // devolve garfos
            sem_post(&forks[left]);
            sem_post(&forks[right]);

            // sai da sala
            sem_post(&room);

            // simula pensar
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));
        }
    }
}

void runSemaphoreMode(std::vector<Philosopher>& philosophers) {
    std::cout << "[MODO SEMAFORO] executando...\n";
    srand(static_cast<unsigned>(time(nullptr)));

    // inicializa semáforos
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1); // cada garfo começa disponível
    }
    sem_init(&room, 0, N - 1); // no máximo 4 filósofos tentando comer

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

    // imprime resultados
    std::cout << "\n=== RESULTADOS MODO SEMAFORO ===\n";
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

    // destrói semáforos
    for (int i = 0; i < N; i++) {
        sem_destroy(&forks[i]);
    }
    sem_destroy(&room);
}

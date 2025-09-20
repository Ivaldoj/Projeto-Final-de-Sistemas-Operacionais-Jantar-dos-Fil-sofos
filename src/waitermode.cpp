#include "waitermode.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <numeric>

namespace {
    std::mutex waiter_mutex;           // o garçom controla tudo
    std::condition_variable waiter_cv; // para acordar filósofos
    bool forks_available[5] = {true, true, true, true, true}; // estado dos garfos
    
    std::mutex stats_mutex;      // protege estatísticas

    const int ROUNDS = 500;      // número fixo de rodadas por filósofo

    struct PhilosopherStats {
        int meals = 0;
        std::vector<long long> wait_times; // em ms
    };

    std::vector<PhilosopherStats> stats(5);

    // Função que verifica se um filósofo pode comer (ambos garfos livres)
    bool canEat(int id) {
        int left = id;
        int right = (id + 1) % 5;
        return forks_available[left] && forks_available[right];
    }

    // Pega os garfos para um filósofo
    void takeForks(int id) {
        int left = id;
        int right = (id + 1) % 5;
        forks_available[left] = false;
        forks_available[right] = false;
    }

    // Libera os garfos de um filósofo
    void releaseForks(int id) {
        int left = id;
        int right = (id + 1) % 5;
        forks_available[left] = true;
        forks_available[right] = true;
    }

    void philosopherLife(int id) {
        for (int count = 0; count < ROUNDS; count++) {
            // marca início da espera
            auto start_wait = std::chrono::high_resolution_clock::now();

            // pede permissão ao garçom para comer
            {
                std::unique_lock<std::mutex> lock(waiter_mutex);
                
                // espera até que o garçom permita comer
                waiter_cv.wait(lock, [id]() { return canEat(id); });
                
                // pega os garfos
                takeForks(id);
            }

            // marca fim da espera
            auto end_wait = std::chrono::high_resolution_clock::now();
            long long wait_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(end_wait - start_wait).count();

            {
                std::lock_guard<std::mutex> lg(stats_mutex);
                stats[id].meals++;
                stats[id].wait_times.push_back(wait_time);
            }

            // simula comendo (fora do lock do garçom!)
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));

            // devolve os garfos ao garçom
            {
                std::lock_guard<std::mutex> lock(waiter_mutex);
                releaseForks(id);
                // notifica todos os filósofos que podem tentar comer
                waiter_cv.notify_all();
            }

            // simula pensando
            std::this_thread::sleep_for(std::chrono::milliseconds(5 + rand() % 10));
        }
    }
}

void runWaiterMode(std::vector<Philosopher>& philosophers) {
    std::cout << "[MODO GARCOM] executando...\n";
    srand(static_cast<unsigned>(time(nullptr))); // inicializa seed do rand

    // reinicia estatísticas
    for (auto& s : stats) {
        s.meals = 0;
        s.wait_times.clear();
    }

    // reinicia estado dos garfos
    for (int i = 0; i < 5; i++) {
        forks_available[i] = true;
    }

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
#include "monitormode.hpp"  
#include "philosopher.hpp"
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
    // Estados possíveis de um filósofo
    enum State { THINKING, HUNGRY, EATING };

    // Monitor que encapsula toda a lógica de sincronização
    class DiningMonitor {
    private:
        mutable std::mutex monitor_mutex;
        std::condition_variable self[5];  // cada filósofo tem sua própria condition_variable
        State state[5];

    public:
        DiningMonitor() {
            for (int i = 0; i < 5; i++) {
                state[i] = THINKING;
            }
        }

        // Testa se um filósofo pode comer
        void test(int id) {
            int left = (id + 4) % 5;  // vizinho à esquerda
            int right = (id + 1) % 5; // vizinho à direita
            
            if (state[id] == HUNGRY && 
                state[left] != EATING && 
                state[right] != EATING) {
                state[id] = EATING;
                self[id].notify_one();
            }
        }

        // Filósofo quer começar a comer
        void pickup_forks(int id) {
            std::unique_lock<std::mutex> lock(monitor_mutex);
            
            state[id] = HUNGRY;
            test(id);  // vê se pode comer imediatamente
            
            // se não pode comer, espera
            while (state[id] != EATING) {
                self[id].wait(lock);
            }
        }

        // Filósofo terminou de comer
        void putdown_forks(int id) {
            std::lock_guard<std::mutex> lock(monitor_mutex);
            
            state[id] = THINKING;
            
            // testa se os vizinhos podem comer agora
            int left = (id + 4) % 5;
            int right = (id + 1) % 5;
            test(left);
            test(right);
        }

        // Para debug - mostra estado atual
        void show_states() const {
            std::lock_guard<std::mutex> lock(monitor_mutex);
            std::cout << "Estados: ";
            for (int i = 0; i < 5; i++) {
                char c = (state[i] == THINKING) ? 'T' : 
                         (state[i] == HUNGRY) ? 'H' : 'E';
                std::cout << i << ":" << c << " ";
            }
            std::cout << std::endl;
        }
    };

    // Instância global do monitor
    DiningMonitor monitor;
    
    std::mutex stats_mutex;      // protege estatísticas

    const int ROUNDS = 500;      // número fixo de rodadas por filósofo

    struct PhilosopherStats {
        int meals = 0;
        std::vector<long long> wait_times; // em ms
    };

    std::vector<PhilosopherStats> stats(5);

    void philosopherLife(int id) {
        for (int count = 0; count < ROUNDS; count++) {
            // marca início da espera
            auto start_wait = std::chrono::high_resolution_clock::now();

            // quer comer - pede ao monitor
            monitor.pickup_forks(id);

            // marca fim da espera
            auto end_wait = std::chrono::high_resolution_clock::now();
            long long wait_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(end_wait - start_wait).count();

            {
                std::lock_guard<std::mutex> lg(stats_mutex);
                stats[id].meals++;
                stats[id].wait_times.push_back(wait_time);
            }

            // simula comendo
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));

            // terminou de comer - informa o monitor
            monitor.putdown_forks(id);

            // simula pensando
            std::this_thread::sleep_for(std::chrono::milliseconds(5 + rand() % 10));
        }
    }
}

void runMonitorMode(std::vector<Philosopher>& philosophers) {
    std::cout << "[MODO MONITOR] executando...\n";
    srand(static_cast<unsigned>(time(nullptr))); // inicializa seed do rand

    // reinicia estatísticas
    for (auto& s : stats) {
        s.meals = 0;
        s.wait_times.clear();
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
    std::cout << "\n=== RESULTADOS MODO MONITOR ===\n";
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
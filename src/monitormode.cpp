#include "monitormode.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <numeric>

namespace {
    const int N = 5;          // número de filósofos
    const int ROUNDS = 200;   // rodadas por filósofo (pode ajustar conforme teste)

    // Estados possíveis de um filósofo
    enum State { THINKING, HUNGRY, EATING };

    // Estrutura para estatísticas de desempenho
    struct PhilosopherStats {
        int meals = 0;                          // refeições realizadas
        std::vector<long long> wait_times;      // tempos de espera (ms)
    };

    // Classe Monitor implementando a lógica do jantar
    class DiningMonitor {
    private:
        std::mutex mtx;                          // mutex para proteger a seção crítica
        std::condition_variable cv[N];           // uma condição por filósofo
        State state[N];                          // estado atual de cada filósofo

    public:
        DiningMonitor() {
            // inicializa todos como "pensando"
            for (int i = 0; i < N; i++) state[i] = THINKING;
        }

        // tenta pegar os dois garfos
        void pickup(int i) {
            std::unique_lock<std::mutex> lock(mtx);
            state[i] = HUNGRY;          // filósofo quer comer
            test(i);                    // verifica se pode comer
            if (state[i] != EATING) {
                // se não puder comer, espera até ser sinalizado
                cv[i].wait(lock, [&]{ return state[i] == EATING; });
            }
        }

        // devolve os garfos após comer
        void putdown(int i) {
            std::unique_lock<std::mutex> lock(mtx);
            state[i] = THINKING;        // volta a pensar
            // verifica se vizinhos agora podem comer
            test((i + N - 1) % N);
            test((i + 1) % N);
        }

    private:
        // checa se o filósofo i pode começar a comer
        void test(int i) {
            int left = (i + N - 1) % N;
            int right = (i + 1) % N;
            if (state[i] == HUNGRY &&
                state[left] != EATING &&
                state[right] != EATING) {
                state[i] = EATING;
                cv[i].notify_one(); // acorda o filósofo i
            }
        }
    };

    // vetor de estatísticas globais
    std::vector<PhilosopherStats> stats(N);

    // função que simula a vida de um filósofo
    void philosopherLife(int id, DiningMonitor& monitor) {
        for (int count = 0; count < ROUNDS; count++) {
            // registra início da espera
            auto start_wait = std::chrono::high_resolution_clock::now();

            monitor.pickup(id); // tenta pegar garfos

            // registra fim da espera
            auto end_wait = std::chrono::high_resolution_clock::now();
            long long wait_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(end_wait - start_wait).count();

            // atualiza estatísticas
            stats[id].meals++;
            stats[id].wait_times.push_back(wait_time);

            // simula comer
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));

            monitor.putdown(id); // devolve garfos

            // simula pensar
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % 20));
        }
    }
} // namespace

// Função principal para rodar o modo Monitor
void runMonitorMode(std::vector<Philosopher>& philosophers) {
    std::cout << "[MODO MONITOR] executando...\n";
    srand(static_cast<unsigned>(time(nullptr)));

    DiningMonitor monitor;

    auto start = std::chrono::high_resolution_clock::now();

    // cria threads para cada filósofo
    std::vector<std::thread> threads;
    for (int i = 0; i < (int)philosophers.size(); i++) {
        threads.emplace_back(philosopherLife, i, std::ref(monitor));
    }

    // espera todas terminarem
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

# Projeto Final de Sistemas Operacionais — Jantar dos Filósofos

Implementação em C++ do problema clássico **Dining Philosophers** com quatro abordagens diferentes de sincronização:

1. **Monitor (mutex + condition variables, estilo Silberschatz)**
2. **Semáforos**
3. **Mutex por garfo**
4. **Garçom / Árbitro**

Cada abordagem foi implementada em arquivos separados e integrada em um menu interativo.

---

## 📂 Estrutura do projeto

```
src/
├── main.cpp
├── philosopher.hpp / philosopher.cpp
├── diningtable.hpp / diningtable.cpp
├── mutexmode.hpp / mutexmode.cpp
├── semaphoremode.hpp / semaphoremode.cpp
├── waitermode.hpp / waitermode.cpp
├── monitormode.hpp / monitormode.cpp
```

---

## ⚙️ Como compilar

No **Linux/WSL/Docker**:

```bash
g++ -std=c++17 -pthread src/*.cpp -o philosophers
```

No **Windows (MinGW)**:

```powershell
g++ -std=c++17 src/*.cpp -o philosophers.exe
```

---

## ▶️ Como executar

Linux/WSL:

```bash
./philosophers
```

Windows (PowerShell):

```powershell
.\philosophers.exe
```

Ao rodar, o programa apresenta o menu:

```
=== Dining Philosophers — Escolha a Solucao ===
1) Monitor (mutex + condvar)
2) Semaforos
3) Mutex por garfo
4) Garcom / Arbitro
0) Sair
>
```

Basta digitar a opção desejada.

---

## 📖 Explicação dos modos

### 1) Monitor (mutex + condvars)
- Baseado no livro *Operating System Concepts* (Silberschatz).
- Cada filósofo tem um estado: `THINKING`, `HUNGRY` ou `EATING`.
- O monitor controla o acesso com `mutex` + `condition_variable`.
- Evita deadlock e starvation, garantindo justiça.

### 2) Semáforos
- Um semáforo para cada garfo.
- Um semáforo global (`room`) limita a **N-1** filósofos simultâneos.
- Evita deadlock garantindo que sempre sobra ao menos um garfo livre.
- Simples, mas aumenta o tempo de espera.

### 3) Mutex por garfo
- Um `std::mutex` por garfo.
- Filósofo tenta pegar os dois mutexes de forma atômica.
- Rápido, mas pode sofrer **starvation** em execuções longas.

### 4) Garçom / Árbitro
- Um árbitro (garçom) controla quem pode comer.
- Filósofos pedem permissão antes de pegar os garfos.
- Evita deadlock e starvation, mantendo bom desempenho.

---

## 📊 Resultados de desempenho

### Garçom / Árbitro
```text
Filosofo 0 -> refeicoes: 200, espera media: 26.915 ms
Filosofo 1 -> refeicoes: 200, espera media: 22.375 ms
Filosofo 2 -> refeicoes: 200, espera media: 22.87 ms
Filosofo 3 -> refeicoes: 200, espera media: 22.06 ms
Filosofo 4 -> refeicoes: 200, espera media: 22.89 ms
Tempo total da simulacao: 13970 ms
```

### Semáforos
```text
Filosofo 0 -> refeicoes: 200, espera media: 21.605 ms
Filosofo 1 -> refeicoes: 200, espera media: 22.735 ms
Filosofo 2 -> refeicoes: 200, espera media: 22.24 ms
Filosofo 3 -> refeicoes: 200, espera media: 23.165 ms
Filosofo 4 -> refeicoes: 200, espera media: 21.755 ms
Tempo total da simulacao: 15526 ms
```

### Mutex
```text
Filosofo 0 -> refeicoes: 200, espera media: 38.115 ms
Filosofo 1 -> refeicoes: 200, espera media: 38.935 ms
Filosofo 2 -> refeicoes: 200, espera media: 41.14 ms
Filosofo 3 -> refeicoes: 200, espera media: 40.89 ms
Filosofo 4 -> refeicoes: 200, espera media: 34.17 ms
Tempo total da simulacao: 13703 ms
```

### Monitor
```text
Filosofo 0 -> refeicoes: 200, espera media: 18.32 ms
Filosofo 1 -> refeicoes: 200, espera media: 19.28 ms
Filosofo 2 -> refeicoes: 200, espera media: 13.245 ms
Filosofo 3 -> refeicoes: 200, espera media: 10.36 ms
Filosofo 4 -> refeicoes: 200, espera media: 13.635 ms
Tempo total da simulacao: 14692 ms
```
---

## 👥 Autores

- Diogo Soares Alves Barreto de Carvalho
- Ivaldo Pureza Freire Júnior

---

## 📌 Observações

- Projeto testado em **Linux Ubuntu 24.04 (Docker)** e **Windows 11 (MinGW)**.
- Para comparação justa, todos os modos executam número fixo de rodadas por filósofo.
- O código está comentado para fins didáticos, explicando a lógica de sincronização em cada solução.

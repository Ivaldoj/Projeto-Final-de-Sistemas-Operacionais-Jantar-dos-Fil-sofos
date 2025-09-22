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
Filosofo 0 -> refeicoes: 500, espera media: 23.822 ms
Filosofo 1 -> refeicoes: 500, espera media: 23.758 ms
Filosofo 2 -> refeicoes: 500, espera media: 21.182 ms
Filosofo 3 -> refeicoes: 500, espera media: 22.730 ms
Filosofo 4 -> refeicoes: 500, espera media: 24.484 ms
Tempo total da simulacao: 33367 ms
```

### Semáforos
```text
Filosofo 0 -> refeicoes: 200, espera media: 47.860 ms
Filosofo 1 -> refeicoes: 200, espera media: 48.940 ms
Filosofo 2 -> refeicoes: 200, espera media: 48.205 ms
Filosofo 3 -> refeicoes: 200, espera media: 47.755 ms
Filosofo 4 -> refeicoes: 200, espera media: 47.780 ms
Tempo total da simulacao: 20578 ms
```

### Mutex
```text
Filosofo 0 -> refeicoes: 500, espera media: 38.780 ms
Filosofo 1 -> refeicoes: 500, espera media: 39.702 ms
Filosofo 2 -> refeicoes: 500, espera media: 37.706 ms
Filosofo 3 -> refeicoes: 500, espera media: 39.382 ms
Filosofo 4 -> refeicoes: 500, espera media: 38.196 ms
Tempo total da simulacao: 33176 ms
```

### Monitor
*(valores variam por execução, exemplo esperado: refeições balanceadas, espera média ~25–30 ms, tempo total ~20–25 s)*

---

## 👥 Autores

- Diogo Soares Alves Barreto de Carvalho
- Ivaldo Pureza Freire Júnior

---

## 📌 Observações

- Projeto testado em **Linux Ubuntu 24.04 (Docker)** e **Windows 11 (MinGW)**.
- Para comparação justa, todos os modos executam número fixo de rodadas por filósofo.
- O código está comentado para fins didáticos, explicando a lógica de sincronização em cada solução.
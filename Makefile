# Nome do executável
TARGET = philosophers

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Se estiver no Linux/WSL/Docker use -pthread
# No Windows (MinGW) pode tirar -pthread se der erro
THREADFLAGS = -pthread

# Diretórios
SRC_DIR = src
OBJ_DIR = build

# Arquivos fonte e objetos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

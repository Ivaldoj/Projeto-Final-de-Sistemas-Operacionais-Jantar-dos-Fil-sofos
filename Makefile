# Nome do executável
TARGET = philosophers

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread

# Fontes
SOURCES = $(wildcard src/*.cpp)

# Regra padrão
all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Limpeza
clean:
	rm -f $(TARGET) *.o

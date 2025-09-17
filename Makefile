# Variáveis
CXX = g++
CXXFLAGS = -O2 -Wall -pthread
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OUT = filosofos

# Regra padrão
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Limpeza
clean:
	rm -f $(OUT)

CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Lista dos arquivos .cpp a serem compilados
SOURCES = UDPSender.cpp UDPServer.cpp TCPSender.cpp TCPServer.cpp

# Gera os nomes dos executáveis a partir dos nomes dos arquivos .cpp
EXECUTABLES = $(SOURCES:.cpp=)

# Regra padrão para compilar todos os executáveis
all: $(EXECUTABLES)

# Regra para cada executável
$(EXECUTABLES): %: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Regra para limpar os arquivos objeto e os executáveis
clean:
	rm -f $(EXECUTABLES) *.o

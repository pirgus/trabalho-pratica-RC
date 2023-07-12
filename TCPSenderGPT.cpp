#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int MAX_BUFFER_SIZE = 1024;

void sendFileTCP(int clientSocket, const std::string& filename, int packetSize) {
    // Abrir o arquivo para leitura
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Falha ao abrir o arquivo" << std::endl;
        return;
    }

    char buffer[MAX_BUFFER_SIZE];

    while (!file.eof()) {
        // Ler o próximo segmento de dados do arquivo
        file.read(buffer, packetSize);
        std::streamsize bytesRead = file.gcount();
        if (bytesRead <= 0) {
            break;
        }

        // Enviar o segmento de dados para o servidor
        ssize_t sentBytes = send(clientSocket, buffer, bytesRead, 0);
        if (sentBytes < 0) {
            std::cerr << "Falha ao enviar o segmento de dados" << std::endl;
            break;
        }
    }

   

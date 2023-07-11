#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Criar o socket UDP
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket < 0) {
        std::cerr << "Falha ao criar o socket UDP" << std::endl;
        return 1;
    }

    // Configurar o endereço do servidor
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9876); // Número da porta do servidor
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Aceitar conexões de qualquer endereço IP

    // Vincular o socket à porta do servidor
    if (bind(udpSocket, (const sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Falha ao vincular o socket UDP ao servidor" << std::endl;
        close(udpSocket);
        return 1;
    }

    // Buffer para armazenar os dados recebidos
    char buffer[1024];

    while (true) {
        // Aguardar a chegada de pacotes UDP
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        ssize_t receivedBytes = recvfrom(udpSocket, buffer, sizeof(buffer), 0,
                                         (sockaddr*)&clientAddress, &clientAddressLength);
        if (receivedBytes < 0) {
            std::cerr << "Falha ao receber o pacote UDP" << std::endl;
            close(udpSocket);
            return 1;
        }

        // Processar os dados recebidos
        std::string receivedData(buffer, receivedBytes);
        std::cout << "Pacote UDP recebido: " << receivedData << std::endl;

        // Responder ao cliente (opcional)
        const char* response = "Hello, UDP client!";
        ssize_t sentBytes = sendto(udpSocket, response, strlen(response), 0,
                                   (const sockaddr*)&clientAddress, sizeof(clientAddress));
        if (sentBytes < 0) {
            std::cerr << "Falha ao enviar a resposta UDP" << std::endl;
            close(udpSocket);
            return 1;
        }
    }

    // Fechar o socket
    close(udpSocket);

    return 0;
}

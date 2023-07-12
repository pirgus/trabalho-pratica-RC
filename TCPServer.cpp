#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>

const size_t BLOCK_SIZE = 100;

int main(int argc, const char** argv) {

    std::ofstream file_received(argv[1], std::ios::binary);

    // Criar o socket TCP
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSocket < 0) {
        std::cerr << "Falha ao criar o socket TCP" << std::endl;
        return 1;
    }

    // Configurar o endereço do servidor
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9876); // Número da porta do servidor
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Aceitar conexões de qualquer endereço IP

    // Vincular o socket à porta do servidor
    if (bind(tcpSocket, (const sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Falha ao vincular o socket TCP ao servidor" << std::endl;
        close(tcpSocket);
        return 1;
    }

    // Ouvir por conexões de clientes
    if (listen(tcpSocket, 5) < 0) { // Permitir até 5 conexões pendentes
        std::cerr << "Falha ao ouvir por conexões TCP" << std::endl;
        close(tcpSocket);
        return 1;
    }

    // Aceitar uma conexão de cliente
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(tcpSocket, (sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        std::cerr << "Falha ao aceitar a conexão TCP" << std::endl;
        close(tcpSocket);
        return 1;
    }

    while (true) {
        // // Aceitar uma conexão de cliente
        // sockaddr_in clientAddress{};
        // socklen_t clientAddressLength = sizeof(clientAddress);
        // int clientSocket = accept(tcpSocket, (sockaddr*)&clientAddress, &clientAddressLength);
        // if (clientSocket < 0) {
        //     std::cerr << "Falha ao aceitar a conexão TCP" << std::endl;
        //     close(tcpSocket);
        //     return 1;
        // }

        // Buffer para armazenar os dados recebidos
        char buffer[BLOCK_SIZE];

        // Receber dados do cliente
        ssize_t receivedBytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (receivedBytes < 0) {
            std::cerr << "Falha ao receber os dados TCP" << std::endl;
            close(clientSocket);
            close(tcpSocket);
            return 1;
        }

        // Processar os dados recebidos
        std::string receivedData(buffer, receivedBytes);
        std::cout << "Dados TCP recebidos: " << receivedData << std::endl;
        file_received.write(receivedData.c_str(), BLOCK_SIZE);

        // Enviar uma resposta ao cliente (opcional)
        // const char* response = "Hello, TCP client!";
        // ssize_t sentBytes = send(clientSocket, response, strlen(response), 0);
        // if (sentBytes < 0) {
        //     std::cerr << "Falha ao enviar a resposta TCP" << std::endl;
        //     close(clientSocket);
        //     close(tcpSocket);
        //     return 1;
        // }

        // Fechar o socket do cliente
        close(clientSocket);
    }

    // Fechar o socket
    close(tcpSocket);
    file_received.close();

    return 0;
}

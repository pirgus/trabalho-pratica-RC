#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Criar o socket TCP
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSocket < 0) {
        std::cerr << "Falha ao criar o socket TCP" << std::endl;
        return 1;
    }

    // Configurar o endereço de destino
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9876); // Número da porta de destino
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereço IP de destino

    // Conectar ao servidor
    if (connect(tcpSocket, (const sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Falha ao conectar ao servidor TCP" << std::endl;
        close(tcpSocket);
        return 1;
    }

    // Enviar dados
    std::string message = "Hello, TCP!";
    ssize_t sentBytes = send(tcpSocket, message.c_str(), message.size(), 0);
    if (sentBytes < 0) {
        std::cerr << "Falha ao enviar o pacote TCP" << std::endl;
        close(tcpSocket);
        return 1;
    }

    std::cout << "Pacote TCP enviado com sucesso" << std::endl;

    // Fechar o socket
    close(tcpSocket);

    return 0;
}

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

const int BLOCK_TO_SEND = 100;

int main(int argc, const char** argv) {

    std::ifstream file_to_send(argv[1], std::ios::binary | std::ios::ate);
    size_t file_size = file_to_send.tellg();
    file_to_send.seekg(0);

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
    serverAddress.sin_addr.s_addr = inet_addr("10.81.71.220"); // Endereço IP de destino

    // Conectar ao servidor
    if (connect(tcpSocket, (const sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Falha ao conectar ao servidor TCP" << std::endl;
        close(tcpSocket);
        return 1;
    }

    // ----------------- copiado do UDP
    size_t readed_bytes = 0;
    char* message = (char*)malloc(BLOCK_TO_SEND * sizeof(char));
    int count = 0;

    while(file_size - readed_bytes > 100){
        file_to_send.read(message, BLOCK_TO_SEND);
        ssize_t sentBytes = sendto(tcpSocket, message, BLOCK_TO_SEND, 0, (const sockaddr*)&serverAddress, sizeof(serverAddress));
        if(sentBytes < 0){
            std::cerr << "Falha ao enviar pacote TCP" << std::endl;
            close(tcpSocket);
            return 1;
        }

        char buffer[BLOCK_TO_SEND];

        socklen_t serverAddressLength = sizeof(serverAddress);

        ssize_t received_bytes = recvfrom(tcpSocket, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddress, &serverAddressLength);
        std::string receivedMessage(buffer, received_bytes);
        std::cout << "Mensagem recebida: " << receivedMessage << std::endl;
    }

    // enviar o que sobrou pois a divisão por 100 não é inteira
    file_to_send.read(message, file_size - readed_bytes);
    ssize_t sentBytes = sendto(tcpSocket, message, file_size - readed_bytes, 0, (const sockaddr*)&serverAddress, sizeof(serverAddress));
    if(sentBytes < 0){
        std::cerr << "Falha ao enviar pacote TCP" << std::endl;
        close(tcpSocket);
        return 1;
    }
    std::cout << "Pacote TCP " << count << " enviado com sucesso" << std::endl;
    // ------------------ copiado do UDP


    // Fechar o socket
    close(tcpSocket);
    file_to_send.close();

    return 0;
}

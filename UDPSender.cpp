#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
    // criando socket udp
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(udpSocket < 0){
        std::cerr << "Falha ao criar o socket UDP" << std::endl;
        return 1;
    }

    // configurar end. de destino
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9876);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // enviando dados
    std::string message = "Hello, UDP";
    ssize_t sentBytes = sendto(udpSocket, message.c_str(), message.size(), 0,
                                 (const sockaddr*)&serverAddress, sizeof(serverAddress));
    if(sentBytes < 0){
        std::cerr << "Falha ao enviar pacote UDP" << std::endl;
        close(udpSocket);
        return 1;
    }

    std::cout << "Pacote UDP enviado com sucesso" << std::endl;

    close(udpSocket);

    return 0;
}
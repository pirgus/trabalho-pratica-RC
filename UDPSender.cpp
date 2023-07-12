#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <cstring>

const int BLOCK_TO_SEND = 100;

int main(int argc, const char** argv){

    const std::string file_name(argv[1]);
    std::ifstream file_to_send(file_name, std::ios::binary | std::ios::ate);
    size_t file_size = file_to_send.tellg();
    //std::cout << "file size = " << file_size << std::endl;
    file_to_send.seekg(0);

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
    serverAddress.sin_addr.s_addr = inet_addr("10.81.71.220");

    size_t readed_bytes = 0;
    char* message = (char*)malloc(BLOCK_TO_SEND * sizeof(char));
    int count = 0;

    while(file_size - readed_bytes > 100){
        file_to_send.read(message, BLOCK_TO_SEND);
        ssize_t sentBytes = sendto(udpSocket, message, BLOCK_TO_SEND, 0, (const sockaddr*)&serverAddress, sizeof(serverAddress));
        if(sentBytes < 0){
            std::cerr << "Falha ao enviar pacote UDP" << std::endl;
            close(udpSocket);
            return 1;
        }

        char buffer[BLOCK_TO_SEND];

        socklen_t serverAddressLength = sizeof(serverAddress);

        ssize_t received_bytes = recvfrom(udpSocket, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddress, &serverAddressLength);
        std::string receivedMessage(buffer, received_bytes);
        //std::cout << "Mensagem recebida: " << receivedMessage << std::endl;
        int compare = strcmp(receivedMessage.c_str(), message);
        std::cout << "Valor da comparaçao = " << compare << std::endl;
        if(compare == 0){
            std::cout << "Pacote UDP " << count << " enviado com sucesso" << std::endl;
            count++;
            readed_bytes += 100;
        }
        else{
            // pacote nao foi enviado corretamente
            std::cout << "Nao foi possível enviar o pacote, tentando novamente...\n";
            // volta o ponteiro do arquivo para no reinício do laço tentar enviar de novo
            size_t go_back = size_t(file_to_send.tellg()) - 100;
            file_to_send.seekg(go_back);
        }
    }

    // enviar o que sobrou pois a divisão por 100 não é inteira
    file_to_send.read(message, file_size - readed_bytes);
    ssize_t sentBytes = sendto(udpSocket, message, file_size - readed_bytes, 0, (const sockaddr*)&serverAddress, sizeof(serverAddress));
    if(sentBytes < 0){
        std::cerr << "Falha ao enviar pacote UDP" << std::endl;
        close(udpSocket);
        return 1;
    }
    std::cout << "Pacote UDP " << count << " enviado com sucesso" << std::endl;



    // // enviando dados
    // std::string message = "Hello, UDP";
    // ssize_t sentBytes = sendto(udpSocket, message.c_str(), message.size(), 0,
    //                              (const sockaddr*)&serverAddress, sizeof(serverAddress));
    // if(sentBytes < 0){
    //     std::cerr << "Falha ao enviar pacote UDP" << std::endl;
    //     close(udpSocket);
    //     return 1;
    // }

    // std::cout << "Pacote UDP enviado com sucesso" << std::endl;

    close(udpSocket);
    file_to_send.close();
    free(message);

    return 0;
}
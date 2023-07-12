#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

const size_t BLOCK_SIZE = 100;

int main(int argc, const char** argv) {
    // Verificar se o nome do arquivo foi fornecido como argumento de linha de comando
    if (argc != 2) {
        std::cerr << "Uso: ./TCPSender <nome_arquivo>" << std::endl;
        return 1;
    }

    // Abrir o arquivo para leitura
    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
        std::cerr << "Falha ao abrir o arquivo" << std::endl;
        return 1;
    }

    // Obter o tamanho do arquivo
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

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

    size_t readed_bytes = 0;
    char buffer[BLOCK_SIZE];

    while(file_size - readed_bytes > 100){
        file.read(buffer, sizeof(buffer));
        ssize_t sent_bytes = send(tcpSocket, buffer, sizeof(buffer), 0);
        if(sent_bytes < 0){
            std::cerr << "Falha ao enviar o pacote TCP" << std::endl;
            size_t go_back = size_t(file.tellg()) - 100;
            file.seekg(go_back);
        }
        readed_bytes += 100;
    }

    // // Enviar o tamanho do arquivo para o servidor
    // ssize_t sentBytes = send(tcpSocket, &file_size, sizeof(file_size), 0);
    // if (sentBytes < 0) {
    //     std::cerr << "Falha ao enviar o tamanho do arquivo" << std::endl;
    //     close(tcpSocket);
    //     return 1;
    // }

    // Enviar o arquivo para o servidor

    // while (!file.eof()) {
    //     file.read(buffer, sizeof(buffer));
    //     ssize_t sentBytes = send(tcpSocket, buffer, file.gcount(), 0);
    //     if (sentBytes < 0) {
    //         std::cerr << "Falha ao enviar o arquivo" << std::endl;
    //         close(tcpSocket);
    //         return 1;
    //     }
    // }

    // Fechar o arquivo
    file.close();

    // Fechar o socket TCP
    close(tcpSocket);

    std::cout << "Arquivo enviado com sucesso" << std::endl;

    return 0;
}

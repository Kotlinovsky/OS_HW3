#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include <iostream>
#include "../common/tools/Network.cpp"

int main(int argc, char *argv[]) {
    std::string server_host = std::string(argv[1]);
    int server_port = atoi(argv[2]);

    // Создадим структуру с описанием адреса сервера.
    struct sockaddr_in serv_addr{0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_host.data());
    serv_addr.sin_port = htons(server_port);

    // Выделяем дескриптор для сокета.
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Server not available!";
        close(socket_fd);
        return 1;
    }

    // Слушаем команды от пользователя.
    while (true) {
        std::string command;

        if (!(std::cin >> command)) {
            break;
        }

        if (command == "close_server") {
            writeByteToFd(socket_fd, 3);
            std::cout << "[MANAGER] Closing server ..." << std::endl;
            break;
        } else if (command == "disconnect_client") {
            uint32_t client_id;
            std::cin >> client_id;
            writeByteToFd(socket_fd, 4);
            writeUInt32ToFd(socket_fd, client_id);
            std::cout << "[MANAGER] Disconnecting client with id = " << client_id << std::endl;
            continue;
        }

        std::cout << "[MANAGER] Command not exists!";
    }

    close(socket_fd);
    return 0;
}

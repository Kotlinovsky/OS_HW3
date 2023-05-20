#include <string>
#include <netinet/in.h>
#include <bits/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <csignal>
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

    // Каждые 2 секунды запрашиваем у сервера его состояние.
    uint32_t single_rooms_busied;
    uint32_t single_rooms_total;
    uint32_t double_rooms_busied;
    uint32_t double_rooms_total;

    while (true) {
        writeByteToFd(socket_fd, 2);

        if (read(socket_fd, &single_rooms_busied, sizeof(uint32_t)) == -1 ||
            read(socket_fd, &single_rooms_total, sizeof(uint32_t)) == -1 ||
            read(socket_fd, &double_rooms_busied, sizeof(uint32_t)) == -1 ||
            read(socket_fd, &double_rooms_total, sizeof(uint32_t)) == -1) {
            break;
        }

        single_rooms_busied = ntohl(single_rooms_busied);
        single_rooms_total = ntohl(single_rooms_total);
        double_rooms_busied = ntohl(double_rooms_busied);
        double_rooms_total = ntohl(double_rooms_total);

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Single rooms: " << single_rooms_busied << "/" << single_rooms_total << std::endl;
        std::cout << "Double rooms: " << double_rooms_busied << "/" << double_rooms_total << std::endl;
        std::cout << "Total rooms: " << single_rooms_busied + double_rooms_busied << "/"
                  << single_rooms_total + double_rooms_total << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        sleep(2);
    }

    close(socket_fd);
    return 0;
}

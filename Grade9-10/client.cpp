#include <csignal>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../common/Client.hpp"
#include "../common/tools/Network.cpp"

/// Обрабатываем закрытие программы, закрываем сокет.
int socket_fd;

void handleTermination([[maybe_unused]] int signal) {
    close(socket_fd);
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cout << "Usage: ./client <id> <gender> <time> <server_host> <server_port>";
        return 1;
    }

    std::string server_host = std::string(argv[4]);
    int server_port = atoi(argv[5]);

    // Создадим структуру с описанием адреса сервера.
    struct sockaddr_in serv_addr{0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_host.data());
    serv_addr.sin_port = htons(server_port);

    // Выделяем дескриптор для сокета.
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    signal(SIGTERM, handleTermination);
    if (connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Server not available!";
        close(socket_fd);
        return 1;
    }

    // Теперь достанем данные о клиенте.
    Client client = strcmp(argv[2], "WOMAN") == 0 || strcmp(argv[2], "woman") == 0 ? WOMAN : MAN;
    time_t rent_time_in_seconds = atol(argv[3]);
    uint32_t client_id = atoi(argv[1]);

    std::cout << "[CLIENT] Client_id = " << client_id << "; client = " << client << "; rent_time = "
              << rent_time_in_seconds
              << std::endl << "[CLIENT] Notifying hotel ..."
              << std::endl;

    // Записываем в сокет информацию о клиенте.
    writeByteToFd(socket_fd, 1);
    writeUInt32ToFd(socket_fd, client_id);
    writeUInt32ToFd(socket_fd, client);
    std::cout << "[CLIENT] Waiting hotel response ..." << std::endl;

    uint8_t status;
    if (read(socket_fd, &status, 1) == -1 || status == false) {
        std::cout << "[CLIENT] Out of service!" << std::endl;
        close(socket_fd);
        return 0;
    }

    std::cout << "[CLIENT] Successfully rent! Waiting " << rent_time_in_seconds << "seconds ..." << std::endl;

    // И теперь висим на соединении все время аренды ...
    // Параллельно проверяем, не разорвали ли соединение
    for (int i = 0; i < rent_time_in_seconds; ++i) {
        sleep(1);
        int error_code;
        int error_code_size = sizeof(error_code);
        getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &error_code, reinterpret_cast<socklen_t *>(&error_code_size));
        if (error_code != 0) {
            std::cout << "[CLIENT] Closed by manager!";
            close(socket_fd);
            return 0;
        }
    }

    close(socket_fd);
    std::cout << "[CLIENT] Rent done!" << std::endl;
    return 0;
}

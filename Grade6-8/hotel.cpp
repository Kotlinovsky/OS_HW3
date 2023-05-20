#include "../common/Hotel.hpp"
#include "../common/tools/Macroses.hpp"
#include "../common/tools/Network.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <algorithm>
#include <iostream>
#include <csignal>
#include <future>

std::mutex mutex;

/// Обрабатывает соединение клиента с сервером.
void handleClient(Hotel &hotel, int client_fd) {
    Room *room = nullptr;
    uint32_t client_id;
    uint8_t pkg_id;
    Client client;

    while (true) {
        if (read(client_fd, (void *) &pkg_id, sizeof(pkg_id)) <= 0) {
            break;
        }

        if (pkg_id == 1) {
            client_id = readUInt32FromFd(client_fd);
            client = static_cast<Client>(readUInt32FromFd(client_fd));
            std::cout << "[SERVER] Connected client, id = " << client_id << "; client = " << client << std::endl;

            // Ищем свободную комнату в отеле.
            mutex.lock();
            room = hotel.serviceClient(client);
            mutex.unlock();

            if (room == nullptr) {
                uint8_t status = false;
                std::cout << "[SERVER] Out of service: client_id = " << client_id << std::endl;
                write(client_fd, &status, 1);
                shutdown(client_fd, SHUT_WR);
                close(client_fd);
                return;
            }

            uint8_t status = true;
            write(client_fd, &status, 1);
            std::cout << "[SERVER] Rent started: client_id = " << client_id << std::endl;
        } else if (pkg_id == 2) {
            mutex.lock();
            std::cout << "[SERVER] Requested state info" << std::endl;
            std::pair<uint32_t, uint32_t> busied = hotel.busiedRooms();
            std::pair<uint32_t, uint32_t> total = hotel.totalRooms();
            writeUInt32ToFd(client_fd, busied.first);
            writeUInt32ToFd(client_fd, total.first);
            writeUInt32ToFd(client_fd, busied.second);
            writeUInt32ToFd(client_fd, total.second);
            mutex.unlock();
        }
    }

    if (room != nullptr) {
        std::cout << "[SERVER] Rent done: client_id = " << client_id << std::endl;
        room->removeOne();
    }
}

/// Обрабатываем закрытие программы, закрываем сокет.
int socket_fd;

void handleTermination([[maybe_unused]] int signal) {
    close(socket_fd);
    socket_fd = -1;
}

int main([[maybe_unused]] int argc, char *argv[]) {
    int port = atoi(argv[1]);

    // Создадим структуру с описанием выделяемого адреса для сервера.
    struct sockaddr_in serv_addr{0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Выделяем дескриптор для сокета.
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    IGNORE(bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)));
    IGNORE(listen(socket_fd, 5));
    signal(SIGTERM, handleTermination);

    // Подготовим переменные для обработки соединений.
    // Также создадим пару объектов для работы логики.
    Hotel hotel(10, 15);
    socklen_t client_address_length;
    std::vector<std::future<void>> tasks;
    struct sockaddr client_address{};

    while (socket_fd != -1) {
        int client_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_address_length);
        if (client_fd == -1) {
            break;
        }

        tasks.push_back(std::async(std::launch::async, handleClient, std::ref(hotel), client_fd));
    }

    // Завершаем задачи на обработку клиентов.
    std::for_each(tasks.begin(), tasks.end(), [](auto &item) { item.get(); });
    return 0;
}

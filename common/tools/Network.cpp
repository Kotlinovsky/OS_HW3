#include <cstdint>
#include <csignal>
#include <netinet/in.h>
#include "Network.hpp"

/// Записывает uint8 в файловый дескриптор.
/// @param fd ID файлового дескриптора.
/// @param value Передаваемое значение.
void writeByteToFd(int fd, uint8_t value) {
    write(fd, (void *) &value, sizeof(value));
}

/// Записывает uint32 в файловый дескриптор.
/// @param fd ID файлового дескриптора.
/// @param value Передаваемое значение.
void writeUInt32ToFd(int fd, uint32_t value) {
    value = htonl(value);
    write(fd, (void *) &value, sizeof(value));
}

/// Читает uint32 из файлового дескриптора.
/// @param fd ID файлового дескриптора.
uint32_t readUInt32FromFd(int fd) {
    uint32_t value;
    read(fd, &value, sizeof(value));
    return ntohl(value);
}

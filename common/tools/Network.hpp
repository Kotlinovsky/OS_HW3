#ifndef IDZ3_NETWORK_HPP
#define IDZ3_NETWORK_HPP

#include <csignal>
#include <netinet/in.h>

/// Записывает uint8 в файловый дескриптор.
/// @param fd ID файлового дескриптора.
/// @param value Передаваемое значение.
void writeByteToFd(int fd, uint8_t value);

/// Записывает uint32 в файловый дескриптор.
/// @param fd ID файлового дескриптора.
/// @param value Передаваемое значение.
void writeUInt32ToFd(int fd, uint32_t value);

/// Читает uint32 из файлового дескриптора.
/// @param fd ID файлового дескриптора.
uint32_t readUInt32FromFd(int fd);

#endif //IDZ3_NETWORK_HPP

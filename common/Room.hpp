#ifndef IDZ3_ROOM_HPP
#define IDZ3_ROOM_HPP

#include "Client.hpp"
#include <vector>
#include <cstdint>

/// Класс, реализующий поведение комнаты.
class Room {
private:
    std::vector<Client> rents;
    std::size_t available;
public:
    /// Конструктор комнаты.
    /// @param available - количество спальных мест в номере.
    explicit Room(std::size_t available);

    /// Бронирует номер для указанного клиента.
    /// @param client - тип клиента.
    void rent(Client client);

    /// Проверяет, можно ли арендовать номер переданному клиенту?
    /// @param client - тип клиента.
    /// @return true - если можно забронировать, иначе false.
    bool canRent(Client client);

    /// Удаляет одну аренду из комнаты.
    void removeOne();

    /// Возвращает количество занятых мест.
    [[nodiscard]]
    uint32_t busied() const;
};

#endif //IDZ3_ROOM_HPP

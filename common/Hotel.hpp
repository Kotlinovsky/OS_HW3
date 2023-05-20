#ifndef IDZ3_HOTEL_HPP
#define IDZ3_HOTEL_HPP

#include <cstdint>
#include "Room.hpp"

/// Класс, описывающий поведение отеля.
class Hotel {
private:
    std::vector<std::vector<Room>> rooms;
public:
    /// Конструктор отеля.
    /// @param single_rooms_count Количество одноместных номеров.
    /// @param double_rooms_count Количество двухместных номеров.
    Hotel(int single_rooms_count, int double_rooms_count);

    /// Обслуживает клиента.
    /// @param client Тип клиента, которого мы хотим заселить.
    Room* serviceClient(Client client);

    /// Возвращает пару с количеством занятых мест в комнатах.
    std::pair<uint32_t, uint32_t> busiedRooms();

    /// Возвращает пару с общим количеством всех мест.
    std::pair<uint32_t, uint32_t> totalRooms();
};

#endif //IDZ3_HOTEL_HPP

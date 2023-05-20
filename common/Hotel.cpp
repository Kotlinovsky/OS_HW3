#include <cstdint>
#include "Hotel.hpp"

Hotel::Hotel(int single_rooms_count, int double_rooms_count) : rooms(2) {
    rooms[0].resize(single_rooms_count, Room(1));
    rooms[1].resize(double_rooms_count, Room(2));
}

Room *Hotel::serviceClient(Client client) {
    for (auto &item: rooms) {
        for (auto &room: item) {
            if (room.canRent(client)) {
                room.rent(client);
                return &room;
            }
        }
    }

    return nullptr;
}

std::pair<uint32_t, uint32_t> Hotel::busiedRooms() {
    uint32_t busied_single = 0;
    uint32_t busied_double = 0;

    for (const auto &item: rooms[0]) {
        busied_single += item.busied();
    }

    for (const auto &item: rooms[1]) {
        busied_double += item.busied();
    }

    return std::make_pair(busied_single, busied_double);
}

std::pair<uint32_t, uint32_t> Hotel::totalRooms() {
    return std::make_pair(rooms[0].size(), rooms[1].size() * 2);
}

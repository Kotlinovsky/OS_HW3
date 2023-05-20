#include "Room.hpp"

Room::Room(std::size_t available) : available(available) {
}

void Room::rent(Client client) {
    if (!canRent(client)) {
        return;
    }

    rents.push_back(client);
}

bool Room::canRent(Client client) {
    if (rents.size() >= available) {
        return false;
    }

    if (!rents.empty() && rents[0] != client) {
        return false;
    }

    return true;
}

uint32_t Room::busied() const {
    return rents.size();
}

void Room::removeOne() {
    rents.pop_back();
}

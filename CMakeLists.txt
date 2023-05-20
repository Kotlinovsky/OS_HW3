cmake_minimum_required(VERSION 3.22)
project(IDZ3)

set(CMAKE_CXX_STANDARD 17)

add_library(Common
        common/Hotel.cpp
        common/Hotel.hpp
        common/Room.cpp
        common/Room.hpp
        common/Client.hpp
        common/tools/Macroses.hpp
        common/tools/Network.hpp
        common/tools/Network.cpp)

add_executable(IDZ3_Grade4-5_Client Grade4-5/client.cpp)
add_executable(IDZ3_Grade4-5_Hotel Grade4-5/hotel.cpp)
target_link_libraries(IDZ3_Grade4-5_Hotel Common)

add_executable(IDZ3_Grade6-8_Monitor Grade6-8/monitor.cpp)
add_executable(IDZ3_Grade6-8_Client Grade6-8/client.cpp)
add_executable(IDZ3_Grade6-8_Hotel Grade6-8/hotel.cpp)
target_link_libraries(IDZ3_Grade6-8_Hotel Common)

add_executable(IDZ3_Grade9-10_Manager Grade9-10/manager.cpp)
add_executable(IDZ3_Grade9-10_Monitor Grade9-10/monitor.cpp)
add_executable(IDZ3_Grade9-10_Client Grade9-10/client.cpp)
add_executable(IDZ3_Grade9-10_Hotel Grade9-10/hotel.cpp)
target_link_libraries(IDZ3_Grade9-10_Hotel Common)
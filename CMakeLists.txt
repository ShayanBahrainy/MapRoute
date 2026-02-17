cmake_minimum_required(VERSION 3.16)
project(TRACEROUTE_GAME LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(traceroute_test src/maproute.cpp src/IP.cpp src/AS.cpp)

add_executable(main src/main.cpp src/ASPath.cpp src/IPV4Path.cpp src/IP.cpp src/AS.cpp)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.12.0) # Replace with your desired version from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(external/raylib)

target_link_libraries(traceroute_test PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(traceroute_test PRIVATE cpr::cpr -licuuc -licui18n)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(main PRIVATE cpr::cpr -licuuc -licui18n)

target_link_libraries(main PRIVATE raylib)
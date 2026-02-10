cmake_minimum_required(VERSION 3.16)
project(TRACEROUTE_GAME LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(maproute src/maproute.cpp src/internet.cpp)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.12.0) # Replace with your desired version from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

target_link_libraries(maproute PRIVATE cpr::cpr -licuuc -licui18n)

cmake_minimum_required(VERSION 3.16)
project(TRACEROUTE_GAME LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(CURL_DISABLE_TESTS ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_FTP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAPS ON CACHE BOOL "" FORCE)
set(CURL_ENABLE_SSL ON CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL ON CACHE BOOL "" FORCE)

set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPR_BUILD_TESTS_SSL OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(libpsl
  GIT_REPOSITORY https://github.com/rockdaboot/libpsl.git
  GIT_TAG 0.21.5)
FetchContent_MakeAvailable(libpsl)

FetchContent_Declare(curl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-8_4_0)
FetchContent_MakeAvailable(curl)

FetchContent_Declare(cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.12.0)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(external/raylib)

add_executable(traceroute_test src/maproute.cpp src/IP.cpp src/AS.cpp)
add_executable(main src/main.cpp src/ASPath.cpp src/IPV4Path.cpp src/IP.cpp src/AS.cpp)

target_link_libraries(traceroute_test PRIVATE nlohmann_json::nlohmann_json cpr::cpr)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json cpr::cpr raylib)

if(UNIX AND NOT APPLE)
  target_link_options(traceroute_test PRIVATE -static-libgcc -static-libstdc++ -licuuc -licui18n)
  target_link_options(main PRIVATE -static-libgcc -static-libstdc++ -licuuc -licui18n)
  target_compile_options(traceroute_test PRIVATE -march=x86-64)
  target_compile_options(main PRIVATE -march=x86-64)
endif()

if(APPLE)
  target_compile_options(traceroute_test PRIVATE -mmacosx-version-min=10.13)
  target_compile_options(main PRIVATE -mmacosx-version-min=10.13)
endif()

if(WIN32)
  set_property(TARGET traceroute_test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set_property(TARGET main PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

install(TARGETS traceroute_test DESTINATION bin)
install(TARGETS main DESTINATION bin)
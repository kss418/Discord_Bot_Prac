cmake_minimum_required(VERSION 3.15)
project(Discord_BotP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(cpr)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows")

find_package(dpp CONFIG REQUIRED)

add_executable(Discord_BotP Http_Request_P.cpp)

target_link_libraries(Discord_BotP PRIVATE dpp::dpp cpr::cpr)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/include")
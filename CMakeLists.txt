cmake_minimum_required(VERSION 3.15)
project(Discord_BotP)
set(CMAKE_TOOLCHAIN_FILE "/mnt/c/Users/jis15/.vscode/Discord_BotP/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(gumbo-query)

add_executable(Discord_BotP Http_Request_P.cpp)

target_link_libraries(Discord_BotP
    PRIVATE
        gumbo
        gumboquery
)

find_package(dpp CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)

target_link_libraries(Discord_BotP
    PRIVATE
        cpr::cpr
        dpp::dpp
)
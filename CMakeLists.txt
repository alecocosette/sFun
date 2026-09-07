cmake_minimum_required(VERSION 3.20)
project(sFun LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch nlohmann/json from GitHub
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# Define the executable
add_executable(sFun
        src/cpu.cpp
        src/main.cpp
        src/Memory.cpp
        src/cart.cpp
        src/CPUflags.cpp
        src/CPUfetch.cpp
        src/CPUstack.cpp
        src/CPUBranch.cpp
        src/CPUjump.cpp
        src/CPUtransfer.cpp
        src/CPUloadstore.cpp
        src/CPUALU.cpp
        src/CPUincrement.cpp
        src/CPUinterrupts.cpp

        src/TestHarness.cpp
        include/Bus.h
        src/Bus.cpp
)

# Link everything
target_include_directories(sFun PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(sFun
        nlohmann_json::nlohmann_json
)
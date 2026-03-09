cmake_minimum_required(VERSION 3.20)
project(sFun LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2 path — pick 64-bit version
set(SDL2_ROOT "C:/Users/yeims/sFun/extra/SDL2/SDL2-2.28.5/x86_64-w64-mingw32")

add_executable(sFun
        src/cpu.cpp
        src/main.cpp
        src/Memory.cpp
        src/cart.cpp

)

# Include headers
target_include_directories(sFun PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${SDL2_ROOT}/include
)

# Link libraries
target_link_directories(sFun PRIVATE
        ${SDL2_ROOT}/lib
)

target_link_libraries(sFun
        SDL2main
        SDL2
)
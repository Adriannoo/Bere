cmake_minimum_required(VERSION 3.16)
project(Bere LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(bere bere_4.c plataforma.c)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(bere PRIVATE -Wall -Wextra -Wpedantic)
    target_link_libraries(bere PRIVATE m)
endif()

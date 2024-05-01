cmake_minimum_required(VERSION 3.5)
project(snekulator C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -Werror -pedantic)

find_package(GSL REQUIRED)

# create a compile_commands.json file for use with clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(snekulator main.c)
target_link_libraries(snekulator GSL::gsl)

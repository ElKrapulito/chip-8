cmake_minimum_required(VERSION 3.16)
project(chip8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")


find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(chip8 WIN32 src/display.c)


target_sources(chip8
	PRIVATE
		src/chip-8.c
		src/keyboard.c
		src/opcodes.c
)


target_link_libraries(chip8 PRIVATE SDL3::SDL3)

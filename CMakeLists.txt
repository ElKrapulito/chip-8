cmake_minimum_required(VERSION 3.16)
project(chip8)

add_executable(chip8)
target_sources(chip8
	PRIVATE
		src/main.c
		src/chip-8.c
		src/opcodes.c
)

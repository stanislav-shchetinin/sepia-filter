cmake_minimum_required(VERSION 3.22)
project(assignment_5_sepia_filter C)

set(CMAKE_C_STANDARD 11)

include_directories(src/include)

enable_language(ASM_NASM)

add_executable(assignment_5_sepia_filter
        src/solution/sepia_asm.asm
        src/include/arguments.h
        src/include/bmp.h
        src/include/image.h
        src/solution/arguments.c
        src/solution/bmp.c
        src/solution/image.c
        src/solution/main.c
        src/include/sepia.h
        src/include/sepia.h
        src/solution/sepia.c
        src/include/comp_time.h
        src/include/comp_time.h
        src/solution/comp_time.c
        src/include/sepia_asm.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -O0")

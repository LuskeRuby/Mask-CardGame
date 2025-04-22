cmake_minimum_required(VERSION 3.30)
project(Mask_CardGame C)

set(CMAKE_C_STANDARD 11)

add_executable(Mask_CardGame
        src/main.c
        src/card.c
        src/playPhase.c
        src/startupPhase.c
)

cmake_minimum_required(VERSION 3.30)
project(Mask_CardGame C)

set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(Mask_CardGame
        src/main.c
        src/deck.c
        src/playPhase.c
        src/startupPhase.c
        src/view/print.c
)

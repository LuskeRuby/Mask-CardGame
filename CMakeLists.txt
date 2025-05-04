cmake_minimum_required(VERSION 3.30)
project(Mask_CardGame C)

set(CMAKE_C_STANDARD 11)


include_directories(${PROJECT_SOURCE_DIR}/src)

# Add the executable target
add_executable(Mask_CardGame
        src/main.c
        src/model/deck.c
        src/controller/playPhase.c
        src/controller/startupPhase.c
        src/view/print.c
        src/model/StartPhaseCommands.c
        src/model/StartPhaseCommands.h
        src/model/playPhaseCommands.c
        src/model/playPhaseCommands.h
        src/view/GUI.c
        src/view/GUI.h
        src/model/playPhaseValidation.c
        src/model/playPhaseValidation.h
)

# Link against the Winsock library
target_link_libraries(Mask_CardGame ws2_32)
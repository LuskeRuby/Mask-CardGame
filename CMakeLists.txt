cmake_minimum_required(VERSION 3.30)
project(Mask_CardGame C)

set(CMAKE_C_STANDARD 11)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include_directories(${PROJECT_SOURCE_DIR}/src)

# Add subdirectory for SDL3 (assuming it's located in vendored/SDL)
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

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
)

# Link to SDL3 library
target_link_libraries(Mask_CardGame PRIVATE SDL3::SDL3)
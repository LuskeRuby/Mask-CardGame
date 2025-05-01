cmake_minimum_required(VERSION 3.30)
project(Mask_CardGame C)

set(CMAKE_C_STANDARD 11)

# Add SDL3 include and lib using relative paths
include_directories(${PROJECT_SOURCE_DIR}/SDL3/include)
link_directories(${PROJECT_SOURCE_DIR}/SDL3/lib)

# Source files
add_executable(Mask_CardGame
        src/main.c
        src/model/deck.c
        src/controller/playPhase.c
        src/controller/startupPhase.c
        src/view/print.c
        src/model/StartPhaseCommands.c
        src/model/playPhaseCommands.c
        src/GUI/gui.c
)

# Include directories
target_include_directories(Mask_CardGame PRIVATE
        src
        src/GUI
        SDL3/include
)

# Link with SDL3
target_link_libraries(Mask_CardGame SDL3)

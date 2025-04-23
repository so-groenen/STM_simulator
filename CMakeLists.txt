cmake_minimum_required(VERSION 3.10.0)
project(stm_simulator VERSION 0.1.0 LANGUAGES C)

set(SOURCES stm_simulator.c 
            game.c
            intro.c
            outro.c
            game_states.c
            game_module.c
            spin_chain.c
            stm.c
            text_module.c
            magnetic_data.c)
add_executable(stm_simulator ${SOURCES})


target_link_libraries     (${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_compile_options    (${PROJECT_NAME} PRIVATE -march=native -fdiagnostics-color)

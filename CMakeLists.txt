cmake_minimum_required(VERSION 3.10.0)
project(stm_simulator VERSION 0.1.0 LANGUAGES C)

set(SOURCES stm_simulator.c 
            src/Scenes/scene_states.c
            src/Scenes/game.c
            src/Scenes/intro.c
            src/Scenes/outro.c
            src/Modules/game_modules.c
            src/Modules/spin_chain.c
            src/Modules/stm.c
            src/Modules/text_module.c
            src/Modules/magnetic_data.c)

add_executable(stm_simulator ${SOURCES})


target_link_libraries     (${PROJECT_NAME} PRIVATE raylib)
# target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options    (${PROJECT_NAME} PRIVATE -march=native -fdiagnostics-color)

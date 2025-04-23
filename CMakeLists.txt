cmake_minimum_required(VERSION 3.10.0)
project(stm_simulator VERSION 0.1.0 LANGUAGES C)

set(SOURCES stm_simulator.c 
            Scenes/src/scene_states.c
            Scenes/src/game.c
            Scenes/src/intro.c
            Scenes/src/outro.c
            Modules/src/game_modules.c
            Modules/src/spin_chain.c
            Modules/src/stm.c
            Modules/src/text_module.c
            Modules/src/magnetic_data.c)

add_executable(stm_simulator ${SOURCES})


target_link_libraries     (${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/Scenes/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/Modules/include)

target_compile_options    (${PROJECT_NAME} PRIVATE -march=native -fdiagnostics-color)

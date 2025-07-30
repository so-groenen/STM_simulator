cmake_minimum_required(VERSION 3.10.0)
project(stm_simulator VERSION 0.1.0 LANGUAGES C)

set(SOURCES src/main.c 
            src/the_game.c
            src/Scenes/scene_states.c
            src/Scenes/game_play.c
            src/Scenes/intro.c
            src/Scenes/outro.c
            src/Modules/game_modules.c
            src/Modules/spin_chain.c
            src/Modules/stm.c
            src/Modules/text_module.c
            src/Modules/magnetic_data.c)

add_executable(stm_simulator ${SOURCES})

find_package(raylib QUIET)
if(raylib_FOUND)
    message("Raylib found.")
else()
    message("Raylib not found, fetching from github")
    include(FetchContent)
    set(RAYLIB_VERSION 5.0)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        FIND_PACKAGE_ARGS
    )
    FetchContent_MakeAvailable(raylib)
endif()
### vv if not platform = web and build inside docs?
set_target_properties(stm_simulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/stm_simulator)

if ("${PLATFORM}" STREQUAL "Web")
    target_compile_definitions(stm_simulator PRIVATE PLATFORM_WEB)
    set_target_properties(stm_simulator PROPERTIES SUFFIX ".html") 
    add_custom_command(
        TARGET stm_simulator PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:stm_simulator>/../resources
    )
    add_custom_command(
        TARGET stm_simulator PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/imgui_shell.html $<TARGET_FILE_DIR:stm_simulator>/../
    )

    #For github pages:
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:stm_simulator>  ${CMAKE_SOURCE_DIR}/docs
    )
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources  ${CMAKE_SOURCE_DIR}/docs/resources
    )
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/docs/stm_simulator.html ${CMAKE_SOURCE_DIR}/docs/index.html
    )
    target_link_options(stm_simulator PRIVATE
                    -sUSE_GLFW=3
                    -sALLOW_MEMORY_GROWTH
                    -sASSERTIONS=1
                    -sWASM=1
                    -sASYNCIFY
                    -sGL_ENABLE_GET_PROC_ADDRESS=1
                    --preload-file resources
                    --shell-file imgui_shell.html)
else()
    add_custom_command(
        TARGET stm_simulator PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:stm_simulator>/resources
    )
endif()
 
 
# set(RAYLIB_DIR C:/pckg/raylib)
# target_link_libraries     (stm_simulator PRIVATE ${RAYLIB_DIR}/src/web/libraylib.web.a)
# target_include_directories(stm_simulator PRIVATE ${PROJECT_SOURCE_DIR}/include ${RAYLIB_DIR}/raylib/include) 
target_include_directories(stm_simulator PRIVATE include) 
target_link_libraries     (stm_simulator PRIVATE raylib)
# inside build-web:
# emcmake cmake .. -DPLATFORM=Web
# mingw32-make
# cmake & make (or Ninja) needs to be in system env variable so that emscripten can find them to build. 
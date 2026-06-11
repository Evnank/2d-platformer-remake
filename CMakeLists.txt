cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(BUILD_SHARED_LIBS OFF)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main 
    src/main.cpp
    src/ASSETS.cpp
    src/GLOBALS.cpp
    src/INPUT.cpp
    src/UI_BUTTON.cpp
    src/enums.cpp
    src/UI.cpp
    src/ENTITY.cpp
    src/PLAYER.cpp
    src/CAMERA.cpp
    src/PERFORMANCE_COUNTER.cpp
    src/GAME.cpp
    )

if(WIN32)
    target_link_options(main PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )
endif()


target_compile_features(main PRIVATE cxx_std_20)



target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

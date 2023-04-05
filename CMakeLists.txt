cmake_minimum_required(VERSION 3.23)
project(conway C)

set(CMAKE_C_STANDARD 99)

# SDL2
set(CMAKE_PREFIX_PATH C:\\MinGW\\include\\SDL2)
find_package(SDL2 REQUIRED)
set(SDL2_INCLUDE_DIR C:\\MinGW\\include\\SDL2\\include)
set(SDL2_LIB_DIR C:\\MinGW\\include\\SDL2\\lib\\x64)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

# SDL_ttf
set(CMAKE_PREFIX_PATH C:\\MinGW\\include\\SDL2_ttf)
find_package(SDL2_ttf REQUIRED)
set(SDL2_ttf_INCLUDE_DIR C:\\MinGW\\include\\SDL2_ttf\\x86_64-w64-mingw32\\include)
set(SDL2_ttf_LIB_DIR C:\\MinGW\\include\\SDL2_ttf\\x86_64-w64-mingw32\\lib)
include_directories(${SDL2_ttf_INCLUDE_DIR})
link_directories(${SDL2_ttf_LIB_DIR})

set(SOURCE_FILES ./src/*.c)
include_directories(.\\include)
file(GLOB SRC_DIR "src/*.c")

add_executable(conway ${SRC_DIR} ./src/main.c)
target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf)
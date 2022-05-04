cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Shooter)

add_library(SDL2_IMAGE SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(SDL2_IMAGE PROPERTIES
  IMPORTED_LOCATION "/usr/local/lib/libSDL2_image.so"  
  INTERFACE_INCLUDE_DIRECTORIES "/usr/local/lib"
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories( ${SDL2_INCLUDE_DIRS} src)

add_executable(ShooterGame src/main.cpp src/game.cpp src/player.cpp src/renderer.cpp 
src/util.cpp src/entity.cpp src/bullet.cpp src/enemy.cpp src/powerup.cpp src/effect.cpp
src/title.cpp src/highscore.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17")
target_link_libraries(ShooterGame stdc++fs)

string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(ShooterGame ${SDL2_LIBRARIES})
target_link_libraries(ShooterGame SDL2_IMAGE) # also adds the required include path
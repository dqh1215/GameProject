cmake_minimum_required(VERSION 3.30)
project(PROJECT_NAME)

set(CMAKE_CXX_STANDARD 20)

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
add_executable(${PROJECT_NAME} src/main.cpp
        src/core/Vector2D.cpp
        src/core/Vector2D.h
        src/core/Timer.cpp
        src/core/Timer.h
        src/entities/Entity.cpp
        src/entities/Entity.h
        src/entities/Character.cpp
        src/entities/Character.h
        src/input/InputHandler.cpp
        src/input/InputHandler.h
        src/Game.cpp
        src/Game.h
        src/graphics/TextureManager.cpp
        src/graphics/TextureManager.h
        src/entities/Bullet.cpp
        src/entities/Bullet.h
        src/entities/Enemy.cpp
        src/entities/Enemy.h)

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL2/SDL2_image/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "SDL2/SDL2_ttf/x86_64-w64-mingw32")
set(SDL2_MIXER_PATH "SDL2/SDL2_mixer/x86_64-w64-mingw32")

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR})
find_package(SDL2_image REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
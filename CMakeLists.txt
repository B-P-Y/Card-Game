cmake_minimum_required(VERSION 3.20)
project(CardGame LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(CardGame)
target_sources(CardGame PRIVATE 
    src/main.cpp
    src/rendering.cpp
    src/UI.cpp
    src/controller.cpp
    src/Home.cpp
    src/klondike.cpp
    src/pyramid.cpp
    src/spider.cpp
    src/freeCell.cpp
    src/animator.cpp)

target_include_directories(CardGame PRIVATE 
"PATH_TO_SDL//C++//SDL//include"
"PATH_TO_SDLImage//SDLImage//SDL3_image-3.2.4//include"
"include")

target_link_libraries(CardGame PRIVATE 
"PATH_TO_SDL3.LIB//SDL//build//Release//SDL3.lib"
"PATH_TO_SDLImage.LIB//SDLImage//SDL3_image-3.2.4//lib//x64//SDL3_image.lib"
"PATH_TO_SDLttf//SDLttf//SDL3_ttf-3.2.2//lib//x64//SDL3_ttf.lib"
)
set_target_properties(CardGame PROPERTIES WIN32_EXECUTABLE FALSE)

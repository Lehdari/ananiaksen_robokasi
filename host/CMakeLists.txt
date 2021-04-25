cmake_minimum_required(VERSION 3.14)
project(RobokasiHost)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(ext)


set(ROBOKASI_HOST_SOURCES ${ROBOKASI_HOST_SOURCES} main.cpp)


add_executable(RobokasiHost ${ROBOKASI_HOST_SOURCES})
target_include_directories(RobokasiHost
    PUBLIC
        ext/
)
target_link_libraries(RobokasiHost
    PUBLIC
        serialib
        gut_opengl
)

cmake_minimum_required(VERSION 3.14)
project(RobokasiHost)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(ext)
add_subdirectory(src)


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
target_compile_definitions(RobokasiHost
    PUBLIC
        ROBOKASI_HOST_RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/"
)

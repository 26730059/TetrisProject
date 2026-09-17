cmake_minimum_required(VERSION 3.11)
project(TetrisRaylib CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------
# raylib da duoc dong goi san trong third_party/raylib
# Khong can Git, khong can internet, chi can CMake + C++ compiler.
# --------------------------------------------------------------------
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/raylib third_party/raylib_build)

# Danh sach source files:
# Tu dong quet tat ca file .cpp trong src/
# Khi cac thanh vien tao class moi (GameLogic.cpp, Renderer.cpp,...),
# CMake se tu dong nhan dien va bien dich cung du an.
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if(NOT SRC_FILES)
    set(SRC_FILES src/main.cpp)
endif()

add_executable(tetris ${SRC_FILES})

target_link_libraries(tetris raylib)

if (APPLE)
    target_link_libraries(tetris "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

# Copy thu muc assets vao cung noi voi file thuc thi sau khi build
add_custom_command(TARGET tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:tetris>/assets)

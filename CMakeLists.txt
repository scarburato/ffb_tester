cmake_minimum_required(VERSION 3.1)
project(ffb_tester)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_STANDARD 14)

# Find SDL2
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED COMPONENTS main)

# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
add_executable(ffb_tester src/main.cpp src/effect_set.cpp)
target_link_libraries(ffb_tester ${SDL2_LIBS})

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET ffb_tester POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:ffb_tester>)
    endforeach()
endif()

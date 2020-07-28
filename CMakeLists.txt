cmake_minimum_required(VERSION 3.1)
project(ffb_tester)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")


###################################################################
#           SNIFFER                                               #
if(NOT WIN32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)

    find_package(PCAP REQUIRED)
    find_package(Threads REQUIRED)

    add_executable(sniffer sniffer/main.cpp sniffer/Sniffer.cpp sniffer/Server.cpp)
    target_link_libraries(sniffer ${PCAP_LIBRARY} Threads::Threads)
endif()

####################################################################
#           SENDER                                                #
# Find SDL2
find_package(SDL2 REQUIRED COMPONENTS main)

# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
add_executable(sender sender/main.cpp sender/Client.cpp sender/effect_set.cpp)
target_link_libraries(sender ${SDL2_LIBS})

# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET sender POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:sender>)
    endforeach()
endif()

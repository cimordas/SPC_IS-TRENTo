cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(root_trento)

set(USE_ROOT TRUE)

if (USE_ROOT)
    message(STATUS "Looking for ROOT")
    find_package(ROOT 6.21 CONFIG REQUIRED)
    message(STATUS "ROOT ${ROOT_VERSION} found at ${ROOT_DIR}.")
    include(${ROOT_USE_FILE})
endif ()

message(STATUS "Looking for BOOST library")
find_package(Boost COMPONENTS filesystem program_options system REQUIRED)
message(STATUS "BOOST include dir found at ${Boost_INCLUDE_DIR}")
link_directories(${Boost_LIBRARY_DIR})


include_directories(${CMAKE_SOURCE_DIR}/src SYSTEM ${Boost_INCLUDE_DIR})
add_subdirectory(src)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#string(APPEND CMAKE_CXX_FLAGS " -std=c++17")
string(APPEND CMAKE_CXX_FLAGS " -lboost_program_options")
string(APPEND CMAKE_CXX_FLAGS " -lstdc++fs")
# string(APPEND CMAKE_CXX_FLAGS " -fopenmp")

message(STATUS "cmake flags are ${CMAKE_CXX_FLAGS}")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "openMP found.")
    string(APPEND CMAKE_CXX_FLAGS " -fopenmp")
    # target_link_libraries(calculate PUBLIC OpenMP::OpenMP_CXX)
endif()

add_executable(root_trento  trento_to_root.cpp)

target_link_libraries(root_trento src ${ROOT_LIBRARIES} ${Boost_LIBRARIES} stdc++fs)

add_executable(merger  merger.cpp)

target_link_libraries(merger src ${ROOT_LIBRARIES} ${Boost_LIBRARIES} stdc++fs)
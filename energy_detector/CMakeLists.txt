cmake_minimum_required(VERSION 3.10)
project(energy_detector)
set(CMAKE_CXXSTANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
list(APPEND CMAKE_CXX_FLAGS "-fPIC")

aux_source_directory(./src SRC_LIBS)

find_package(OpenCV REQUIRED)

add_executable(energy_detector main.cpp)

add_library(SRC SHARED
       	    ${SRC_LIBS})

target_include_directories(SRC 
                          PUBLIC include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(SRC ${OpenCV_LIBS})
target_include_directories(energy_detector PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(energy_detector ${OpenCV_LIBS} SRC)

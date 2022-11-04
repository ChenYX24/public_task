cmake_minimum_required(VERSION 3.10)
project(armor_detector)
set(CMAKE_CXXSTANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
list(APPEND CMAKE_CXX_FLAGS "-fPIC")
# 查找 GTest 库
find_package(GTest REQUIRED)
# 链接gtest
target_link_libraries(GTest::GTest INTERFACE gtest_main)
add_executable(armor_detector_test ./test/armor_detector_test.cpp)
aux_source_directory(./src SRC_LIBS)

find_package(OpenCV REQUIRED)

add_executable(armor_detector main.cpp)

add_library(SRC SHARED
       	    ${SRC_LIBS})

target_include_directories(SRC 
                          PUBLIC include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(SRC ${OpenCV_LIBS})

target_include_directories(armor_detector PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(armor_detector ${OpenCV_LIBS} SRC)

target_include_directories(armor_detector_test PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(armor_detector_test GTest::GTest ${OpenCV_LIBS} SRC)
# 增加测试
add_test(armor_detector_tests armor_detector_test)
# 开启测试
enable_testing()
# 添加测试目录
add_subdirectory(test)




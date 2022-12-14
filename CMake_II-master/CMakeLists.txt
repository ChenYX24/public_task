cmake_minimum_required(VERSION 3.10)
project(Test)
set(CMAKE_CXXSTANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
list(APPEND CMAKE_CXX_FLAGS "-fPIC")
find_package(OpenCV REQUIRED)
add_executable(server server.cpp)
add_executable(client client.cpp)
add_executable(assembly1_test ./modules/assembly1/test/assembly1_test.cpp)
# # # # 以下内容禁止修改 # # # #
option(BUILD_A "build modules a" ON)
option(BUILD_B "build modules b" OFF)
option(BUILD_TESTS "build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(modules)
add_subdirectory(common)
# 小提示: module2 需要用到 pthread，即 target_link_libraries(xxx pthread)
# # # # 以上内容禁止修改 # # # #
target_link_libraries(assembly1_test MODULES pthread)
target_include_directories(server PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(server ${OpenCV_LIBS} MODULES COMMON pthread)
target_include_directories(client PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(client ${OpenCV_LIBS} MODULES COMMON pthread)

if (BUILD_A) 
	add_definitions(-DWITH_A)
endif()
if (BUILD_B) 
	add_definitions(-DWITH_B)
endif()

ADD_TEST(NAME assembly1_test COMMAND assembly1_test)
SET_TESTS_PROPERTIES(assembly1_test PROPERTIES PASS_REGULAR_EXPRESSION "2")
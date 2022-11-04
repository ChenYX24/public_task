# # 查找 GTest 库
# find_package(GTest REQUIRED)

# find_package(OpenCV REQUIRED)

# # 链接gtest
# target_link_libraries(GTest::GTest INTERFACE gtest_main)


# # 将我们写的tests编译成可执行文件
# add_executable(armor_detector_test armor_detector_test.cpp)
# aux_source_directory(../src SRC)
# add_library(armor_detector_test1 ${SRC})

# target_include_directories(
#   armor_detector_test1 PUBLIC ../include
#   ${OpenCV_INCLUDE_DIRS}
# )

# # 把GTest和被测函数都链接到 可执行文件 上
# target_link_libraries(armor_detector_test
#   PUBLIC
#   GTest::GTest
#   ${OpenCV_LIBS}
#   ${armor_detector_test1}
#   )



# # 增加测试
# add_test(armor_detector_gtests armor_detector_test)
# find_package(OpenCV REQUIRED)
# aux_source_directory(../src SRC)
# add_library(armor_detector_test SHARED
#             ${SRC}
#             ./armor_detector_test.cpp)
# # 查找 GTest 库
# find_package(GTest REQUIRED)
# # GTest 的头文件
# include_directories(${GTEST_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

# # 链接测试库

# target_include_directories(
#   armor_detector_test PUBLIC
#   ../include
#   ${OpenCV_LIBS}
# )

# # 添加到测试
# gtest_discover_tests(armor_detector_test)

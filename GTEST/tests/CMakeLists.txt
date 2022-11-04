# 查找 GTest 库
find_package(GTest REQUIRED)
# 链接gtest
target_link_libraries(GTest::GTest INTERFACE gtest_main)


# 将我们写的tests编译成可执行文件
add_executable(multiply_test multiply_test.cpp)

# 把GTest和被测函数都链接到 可执行文件 上
target_link_libraries(multiply_test
 PRIVATE
  GTest::GTest
  multiply)

# 增加测试
add_test(multiply_gtests multiply_test)

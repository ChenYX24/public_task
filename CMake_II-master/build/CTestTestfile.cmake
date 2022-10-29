# CMake generated Testfile for 
# Source directory: /home/cyx/CMake_II-master
# Build directory: /home/cyx/CMake_II-master/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(assembly1_test "/home/cyx/CMake_II-master/build/assembly1_test")
set_tests_properties(assembly1_test PROPERTIES  PASS_REGULAR_EXPRESSION "2" _BACKTRACE_TRIPLES "/home/cyx/CMake_II-master/CMakeLists.txt;37;ADD_TEST;/home/cyx/CMake_II-master/CMakeLists.txt;0;")
subdirs("modules")
subdirs("common")

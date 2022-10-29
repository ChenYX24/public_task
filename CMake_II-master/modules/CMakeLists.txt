find_package(OpenCV REQUIRED)
aux_source_directory(./assembly1/src MODULES_LIB_DIR1)
aux_source_directory(./assembly1/test MODULES_LIB_DIR2)
aux_source_directory(./assembly2/src MODULES_LIB_DIR3)
aux_source_directory(./module1/src/module1a  MODULES_LIB_DIR4)
aux_source_directory(./module1/src/module1b  MODULES_LIB_DIR5)
aux_source_directory(./module2/src MODULES_LIB_DIR6)


add_library(MODULES SHARED
       	${MODULES_LIB_DIR1} ${MODULES_LIB_DIR2}
	${MODULES_LIB_DIR3} ${MODULES_LIB_DIR4}
	${MODULES_LIB_DIR5} ${MODULES_LIB_DIR6}
	)



target_include_directories(
        MODULES PUBLIC
        ./assembly1/include
        ./assembly2/include
        ./module1/include
        ./module1/include/robotlab/module1
        ./module2/include
        ./module2/include/robotlab/opcua_cs
        ./module2/include/robotlab/opcua_cs/ua_type
        ${OpenCV_INCLUDE_DIRS}
        )
target_link_libraries(MODULES ${OpenCV_LIBS})

find_package(OpenCV REQUIRED)
aux_source_directory(./rmath/src COMMON_LIB_DIR1)
add_library(COMMON SHARED
	${COMMON_LIB_DIR1}
	)


target_include_directories(
        COMMON PUBLIC
        ./singleton/include
        ./rmath/include
        ${OpenCV_INCLUDE_DIRS}
        )

target_link_libraries(COMMON ${OpenCV_LIBS})

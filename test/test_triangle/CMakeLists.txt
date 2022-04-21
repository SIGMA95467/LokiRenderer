cmake_minimum_required (VERSION 3.5)


project(test_triangle)


set(CMAKE_CXX_STANDARD 11)

include_directories(../../include)
include_directories(../../external/include)



# 指定可执行程序输出目录
set(publish_bin_debug 			${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>)
set(publish_bin_release 		${CMAKE_BINARY_DIR}/$<$<CONFIG:Release>:Release>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${publish_bin_debug})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${publish_bin_release})



# Create the executable
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    LokiRenderer
)






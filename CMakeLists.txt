# CMakeList.txt : CMake project for LokiRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)
project ("LokiRenderer")



# Add source to this project's executable.


# TODO: Add tests and install targets if needed.






include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/external/include)

link_directories(${PROJECT_SOURCE_DIR}/external/libs)


# file(GLOB_RECURSE SRCS ./src_Test/*.cpp)
file(GLOB_RECURSE SRCS ./src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

source_group("Header Files" FILES ${HEADERS})

add_library(${PROJECT_NAME} ${SRCS} ${HEADERS})



target_link_libraries( ${PROJECT_NAME} 
				PUBLIC 
)


# Add sub directories
add_subdirectory(test/test_triangle)

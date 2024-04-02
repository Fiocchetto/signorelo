cmake_minimum_required(VERSION 3.23)

set( CMAKE_CXX_COMPILER "clang++" )
set( CMAKE_C_COMPILER "clang" )

project(Signorello)

include_directories(
	${Signorello_SOURCE_DIR}/include
)

add_executable(Main "${Signorello_SOURCE_DIR}/src/main.cpp")

target_link_libraries(Main -lgmp -lgmpxx -lmpfr )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
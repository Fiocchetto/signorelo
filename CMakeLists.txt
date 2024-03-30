cmake_minimum_required(VERSION 3.23)

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_C_COMPILER "gcc" )

project(Signorello)

include_directories(
	${Signorello_SOURCE_DIR}/include
)

add_executable(Main "${Signorello_SOURCE_DIR}/src/main.cpp")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
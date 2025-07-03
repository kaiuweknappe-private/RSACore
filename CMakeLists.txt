
cmake_minimum_required(VERSION 3.10)
project(systemc_project)

set(CMAKE_CXX_STANDARD 17)

# PATH TO SYSTEMC INSTALL
set(SYSTEMC_HOME "$ENV{HOME}/systemc-install")

include_directories(${SYSTEMC_HOME}/include)
link_directories(${SYSTEMC_HOME}/lib-linux64)

add_executable(sim 
	src/main.cpp
	src/MontgomeryMul.cpp
)
target_link_libraries(sim systemc)

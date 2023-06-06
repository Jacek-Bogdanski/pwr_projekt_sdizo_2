cmake_minimum_required(VERSION 3.24)
project(pwr_projekt_sdizo_2)

#Set Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

set(CMAKE_CXX_STANDARD 11)

add_executable(pwr_projekt_sdizo_2 main.cpp src/GraphMatrix.cpp src/GraphMatrix.h src/GraphList.cpp src/ShortestPathReturn.cpp src/ShortestPathReturn.h src/MenuHandler.cpp)

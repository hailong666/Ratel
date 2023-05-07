cmake_minimum_required(VERSION 3.22.1)
project(mymuduo)

# cmake => makefile  make
#mymuduo 最终编译成so动态库的路径,放在根目录的lib下
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置调试信息 以及 C++11标准
#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")

#定义参与编译的源代码文件
aux_source_directory(. SRC_LIST)

add_library(mymuduo SHARED ${SRC_LIST})

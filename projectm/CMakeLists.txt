PROJECT(projectM-complete)
cmake_minimum_required(VERSION 2.4.0)
cmake_policy(SET CMP0005 OLD)

if(COMMAND cmake_policy)
       cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


include(src/cmake/CPack-projectM.cmake)

add_subdirectory(src)

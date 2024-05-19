cmake_minimum_required(VERSION 3.0.0)
project(pyd-smc-ctf-challenge VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "-masm=intel")

add_library(decider SHARED decider.cpp smc.cpp smc.h)
add_library(prepare SHARED prepare.cpp smc.cpp smc.h)

set(PYTHON37_LIB C:/python/libs)
include_directories(C:/python/include)
target_link_libraries(decider ${PYTHON37_LIB}/python37.lib)
set_target_properties(decider PROPERTIES OUTPUT_NAME decider.pyd PREFIX "")
target_link_libraries(prepare ${PYTHON37_LIB}/python37.lib)
set_target_properties(prepare PROPERTIES OUTPUT_NAME prepare.pyd PREFIX "")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

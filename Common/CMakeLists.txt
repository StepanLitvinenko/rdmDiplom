cmake_minimum_required(VERSION 3.0)
project (Common)

include_directories(${PROC_LIBS_ROOT})
file(GLOB  SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
add_library(${PROJECT_NAME} STATIC  ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME})


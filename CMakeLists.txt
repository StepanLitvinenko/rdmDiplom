cmake_minimum_required(VERSION 2.8)

project(RdmDiplom)
#set(SOURCE_EXE main.cpp)
include_directories(Common)
#add_executable(main ${SOURCE_EXE})
add_executable(${PROJECT_NAME} "main.cpp")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Common Common)
target_link_libraries(${PROJECT_NAME} Common)

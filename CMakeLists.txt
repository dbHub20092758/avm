
cmake_minimum_required(VERSION 3.5)

project(avm)

include_directories(${CMAKE_CURRENT_DIR})

# opencv
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "opencv is: ${OpenCV_INCLUDE_DIRS}")

# executable
add_executable(test main.cpp)
target_link_libraries(test
 glog 
 ${OpenCV_LIBS})
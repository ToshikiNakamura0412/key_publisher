cmake_minimum_required(VERSION 3.0.2)
project(key_publisher)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
 CATKIN_DEPENDS roscpp std_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(key_publisher src/key_publisher.cpp)
target_link_libraries(key_publisher ${catkin_LIBRARIES})

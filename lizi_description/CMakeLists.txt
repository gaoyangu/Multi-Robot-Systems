cmake_minimum_required(VERSION 2.8.3)
project(lizi_description)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package()

install(
  DIRECTORY launch materials meshes models robots urdf worlds
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
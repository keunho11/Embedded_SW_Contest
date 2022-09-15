#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(RealSense_Dlib)

find_package( OpenCV 4 REQUIRED )
find_package(realsense2 REQUIRED)
find_package(   dlib REQUIRED   )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(ISP_socket ISP_socket.cpp )
set_property(TARGET ISP_socket PROPERTY CXX_STANDARD 11)
target_link_libraries(ISP_socket ${OpenCV_LIBS} ${realsense2_LIBRARY} dlib::dlib)

#set_target_properties (rs-face-dlib PROPERTIES
#	FOLDER "Examples/dlib"
#)

#install(
#	TARGETS
#
#	rs-face-dlib
#
#	RUNTIME DESTINATION
#	${CMAKE_INSTALL_PREFIX}/bin
#)

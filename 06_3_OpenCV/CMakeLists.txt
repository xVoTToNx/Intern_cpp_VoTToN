cmake_minimum_required(VERSION 3.12)

set(OpenCV_DIR c:/opencv-master/build/install)
set (OPENCV_ENABLE_NONFREE TRUE)

project( KeyPoints )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories("files")

add_executable( KeyPoints files/main.cpp)
target_link_libraries( KeyPoints ${OpenCV_LIBS} )
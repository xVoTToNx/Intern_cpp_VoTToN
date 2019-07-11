cmake_minimum_required(VERSION 3.12)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(OpenCV_DIR c:/OpenCV/opencv/build/x64/vc14/lib)

project( OpenCVProj )

find_package( OpenCV REQUIRED )
set(OpenCV_FOUND TRUE)
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories("files")

add_executable( OpenCVProj files/main.cpp)
target_link_libraries( OpenCVProj ${OpenCV_LIBS} )
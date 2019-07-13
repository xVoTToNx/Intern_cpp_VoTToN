cmake_minimum_required(VERSION 3.12)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(OpenCV_DIR c:/OpenCV/opencv/build/x64/vc14/lib)

project( MoveDePikcha )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories("files")

add_executable( MoveDePikcha files/main.cpp)
target_link_libraries( MoveDePikcha ${OpenCV_LIBS} )
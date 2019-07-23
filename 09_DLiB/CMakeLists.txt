cmake_minimum_required(VERSION 2.8.12)

project(FacePose)

add_subdirectory(../dlib dlib_build)
set(OpenCV_DIR c:/opencv-master/build/install)

macro(add_example name)
   add_executable(${name} ${name}.cpp)
   target_link_libraries(${name} dlib::dlib )
endmacro()

if (DLIB_NO_GUI_SUPPORT)
   message("No GUI support, so we won't build the webcam_face_pose_ex example.")
else()
   find_package(OpenCV REQUIRED)
   if (OpenCV_FOUND)
      include_directories(${OpenCV_INCLUDE_DIRS})

      add_executable(FacePose FacePose.cpp)
      target_link_libraries(FacePose dlib::dlib ${OpenCV_LIBS} )
   else()
      message("OpenCV not found, so we won't build the webcam_face_pose_ex example.")
   endif()
endif()



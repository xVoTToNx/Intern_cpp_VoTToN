cmake_minimum_required(VERSION 3.12)

project( MyBoi )

set(WITH_BOOST "c:/Users/User/Desktop/boost_1_70_0/boost_1_70_0")
include_directories("files")

include_directories("c:/Program Files (x86)/MySQL/Connector C++ 8.0/include")
include_directories("c:/Users/User/Desktop/boost_1_70_0/boost_1_70_0")
include_directories("c:/Program Files (x86)/MySQL/Connector C++ 8.0/lib")

add_executable( MyBoi files/main.cpp)

link_libraries(MyBoi  "c:/Program Files/MySQL/Connector C++ 8.0/lib64/vs14")
target_link_libraries(MyBoi  
"c:/Program Files (x86)/MySQL/Connector C++ 8.0/lib/vs14/mysqlcppconn-static.lib")
target_link_libraries(MyBoi  
"c:/Program Files (x86)/MySQL/Connector C++ 8.0/lib/vs14/mysqlcppconn8-static.lib")
target_link_libraries(MyBoi  
"c:/Program Files (x86)/MySQL/Connector C++ 8.0/lib/vs14/mysqlcppconn8.lib")
target_link_libraries(MyBoi  
"c:/Program Files (x86)/MySQL/Connector C++ 8.0/lib/vs14/mysqlcppconn.lib")
target_link_libraries(MyBoi  
"c:/Program Files (x86)/MySQL/Connector C++ 8.0/lib/vs14/libeay32.lib")
target_link_libraries(MyBoi  
"c:/Program Files (x86)/MySQL/Connector C++ 8.0/lib/vs14/ssleay32.lib")
link_libraries(MyBoi  "c:/Users/User/Desktop/boost_1_70_0/boost_1_70_0")

cmake_minimum_required(VERSION 3.10.2)
project (mac_gfdm)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pthread")
set(CMAKE_LINKER_FLAGS "-pthread")

set(dir_scratch  "scratch/") # run files (for executables)
set(dir_build "build/")      # executables
set(dir_src "src/")          # class files

set(EXECUTABLE_OUTPUT_PATH ${dir_build})
set(CMAKE_BUILD_DIRECTORY ${dir_build})

set(src_files ${dir_src}channel_server.h 
              ${dir_src}channel_server.cpp 
              ${dir_src}phy_interface.h
              ${dir_src}phy_interface.cpp
              ${dir_src}phy_client.h 
              ${dir_src}phy_client.cpp 
              ${dir_src}mac.h 
              ${dir_src}mac.cpp 
              ${dir_src}mac_ue.h 
              ${dir_src}mac_ue.cpp 
              ${dir_src}mac_base_station.h
              ${dir_src}mac_base_station.cpp
              ${dir_src}mac_queue.h
              ${dir_src}mac_queue.cc
              ${dir_src}app.h 
              ${dir_src}app.cpp
              ${dir_src}app_ue.h 
              ${dir_src}app_ue.cpp
              ${dir_src}app_base_station.h
              ${dir_src}app_base_station.cpp
              ${dir_src}datagram.h
              ${dir_src}datagram.cpp) # set source files (classes)


include_directories(src) #Bring the headers files

add_executable(channel_server.exe ${dir_scratch}channel_server.cpp ${src_files})
add_executable(ue_node.exe ${dir_scratch}ue_node.cpp ${src_files})
add_executable(base_station_node.exe ${dir_scratch}base_station_node.cpp ${src_files})

add_custom_target(runtest ./run_example.sh)

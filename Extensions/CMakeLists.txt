cmake_minimum_required(VERSION 3.0)

project(augmented-reality)

set(CMAKE_CXX_STANDARD 11)

# OpenCV library
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# main executable
add_executable(main_extend main_extend.cpp extend_helper.cpp helper_csv_extend.cpp)
target_link_libraries(main_extend ${OpenCV_LIBS})

add_executable(extend_helper extend_helper.cpp  main_extend.cpp helper_csv_extend.cpp)
target_link_libraries(extend_helper ${OpenCV_LIBS})

add_executable(helper_csv_extend  extend_helper.cpp  main_extend.cpp helper_csv_extend.cpp)
target_link_libraries(helper_csv_extend ${OpenCV_LIBS})


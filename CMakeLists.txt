cmake_minimum_required(VERSION 3.0)

project(augmented-reality)

set(CMAKE_CXX_STANDARD 11)

# OpenCV library
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# main executable
add_executable(main main.cpp calibration.cpp 3D_projection.cpp helper_csv.cpp)
target_link_libraries(main ${OpenCV_LIBS})

# calibration executable
add_executable(calibration calibration.cpp main.cpp 3D_projection.cpp helper_csv.cpp)
target_link_libraries(calibration ${OpenCV_LIBS})

# project executable
add_executable(3D_projection 3D_projection.cpp calibration.cpp  3D_projection.h main.cpp helper_csv.cpp)
target_link_libraries(3D_projection ${OpenCV_LIBS})

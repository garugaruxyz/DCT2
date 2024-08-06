cmake_minimum_required(VERSION 3.28)
project(Metodi_del_Calcolo_Scientifico_2)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(Metodi_del_Calcolo_Scientifico_2
        src/main.cpp
        src/utils/utils.h
        src/utils/utils.cpp)

target_link_libraries(Metodi_del_Calcolo_Scientifico_2 ${OpenCV_LIBS})
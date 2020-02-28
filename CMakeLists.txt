cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ChangeResolution
  VERSION 0.0.1
  DESCRIPTION "ChangeResolution"
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")
set(TARGET_NAME "ChangeResolution")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_PREFIX_PATH /home/gmatczak/opencv/build)

configure_file(config.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json)


find_package(Qt5Core REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
#include fix for OpenCV
include_directories(
    ${OpenCV_INCLUDE_DIRS}
)


set(SRC
  src/changeresolution.cc
  ../IncludeSpdlog/spdlog.cc
  ../ConfigReader/src/configreader.cc
)
set(HEADERS
  ../IncludeSpdlog/spdlog.h
  ../ConfigReader/src/configreader.h
)

add_executable( 
${TARGET_NAME}    
${SRC}   
 ${HEADERS}  )


target_link_libraries(${PROJECT_NAME}  Qt5::Core  ${OpenCV_LIBS} Threads::Threads )
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(Crop
  VERSION 0.0.1
  DESCRIPTION "Crop"
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")
set(TARGET_NAME "Crop")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_PREFIX_PATH /home/gmatczak/opencv/build)

if(ZUT)
  message(STATUS "ZUT Selected")
  set(OpenCV_DIR /media/genetic/opencv/buildQTOFF)
  set(CMAKE_PREFIX_PATH /media/genetic/opencv/buildQTOFF)
  set(Qt5Core_DIR   /media/genetic/Qt/5.12.5/gcc_64/lib/cmake/Qt5Core)
  configure_file(config_ZUT.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json)
endif()

if(HOME)
  message(STATUS "HOME Selected")
  configure_file(config_HOME.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json)
endif()
if(NOVOICE)
  message(STATUS "NOVOICE Selected")
  configure_file(config_NOVOICE.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json)
endif()

if(VIDEOTOIMAGE)
  message(STATUS "VIDEOTOIMAGE Selected")
  configure_file(config_VIDEOTOIMAGE.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json)
endif()


find_package(Qt5Core REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
#include fix for OpenCV
include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

if(HOME)
set(SRC
  src/main.cc
  ../IncludeSpdlog/spdlog.cc
)
endif()

if(NOVOICE)
set(SRC
  src/mainNoVoice.cc
  ../IncludeSpdlog/spdlog.cc
)
endif()

if(VIDEOTOIMAGE)
set(SRC
  src/videoToImage.cc
  ../IncludeSpdlog/spdlog.cc
)

endif()

set(HEADERS
  ../IncludeSpdlog/spdlog.h
)

add_executable(${TARGET_NAME}
    ${SRC}
    ${HEADERS}
)

if(HOME)
target_link_libraries(${PROJECT_NAME}  Qt5::Core  ${OpenCV_LIBS} Threads::Threads )
endif()

if(NOVOICE)
target_link_libraries(${PROJECT_NAME}  Qt5::Core  ${OpenCV_LIBS} Threads::Threads)
endif()

if(VIDEOTOIMAGE)
target_link_libraries(${PROJECT_NAME}  Qt5::Core  ${OpenCV_LIBS} Threads::Threads)
endif()

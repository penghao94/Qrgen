cmake_minimum_required(VERSION 2.8.12)

project(example)

#add qrgen lib

set(QR_INCLUDE_DIR ${QR_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/qrgen)
include_directories(${QR_INCLUDE_DIR})
add_subdirectory(qrgen)
set (QR_LIBS ${QR_LIBS} qrgen)

#add priject files

FILE(GLOB SRCFILES *.cpp)
FILE(GLOB HEADFILES *.h)

add_executable(${PROJECT_NAME}_bin ${SRCFILES} ${HEADFILES})
target_link_libraries(${PROJECT_NAME}_bin ${QR_LIBS})
cmake_minimum_required(VERSION 3.19...3.27 FATAL_ERROR)

project(Exercise1)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIRS}) 

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(ex1 ex1.cc ${sources} ${headers})
target_link_libraries(ex1 ${Geant4_LIBRARIES})


add_custom_target(Exercise DEPENDS ex1)

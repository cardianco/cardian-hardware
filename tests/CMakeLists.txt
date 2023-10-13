cmake_minimum_required(VERSION 3.26)

file(GLOB source-files *.cpp)

project(tests CXX)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

add_subdirectory(${PROJECT_SOURCE_DIR}/../main/utils ${CMAKE_CURRENT_BINARY_DIR}/utils)
add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/googletest)

foreach(testx ${source-files})
    get_filename_component(basename ${testx} NAME_WE)
    message(${basename})
    add_executable(${basename} timer-test.cpp)
    add_test(NAME ${basename} COMMAND ${basename})
    target_link_libraries(${basename} lib-utils gtest)
endforeach()

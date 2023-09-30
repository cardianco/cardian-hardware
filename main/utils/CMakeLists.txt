cmake_minimum_required(VERSION 3.26)

file(GLOB sources *.h *.cpp)

add_library(lib-utils STATIC
    ${sources}
)
target_include_directories(lib-utils PUBLIC .)
set_target_properties(lib-utils PROPERTIES LINKER_LANGUAGE CXX)


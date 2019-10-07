file(GLOB_RECURSE files "src/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")

add_library(core STATIC ${files})

target_include_directories(core PUBLIC include)

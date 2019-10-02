set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

add_subdirectory(core)

file(GLOB_RECURSE files "src/*.*")

add_library(sprinter STATIC ${files})

target_include_directories(sprinter PUBLIC include)
target_link_libraries(sprinter PRIVATE core)

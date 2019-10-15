file(GLOB_RECURSE files "src/*.cpp")

add_library(sprinter STATIC ${files})

target_include_directories(sprinter PUBLIC include)
target_link_libraries(sprinter PRIVATE core transport)

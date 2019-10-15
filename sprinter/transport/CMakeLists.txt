file(GLOB_RECURSE files "src/*.cpp")

add_library(transport STATIC ${files})

target_include_directories(transport PUBLIC include)

target_link_libraries(transport PRIVATE core)

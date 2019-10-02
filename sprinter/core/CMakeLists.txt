file(GLOB_RECURSE files "src/*.*")

add_library(core STATIC ${files})

target_include_directories(core PUBLIC include)

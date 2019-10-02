set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} \
-Os \
-fno-exceptions \
-fno-non-call-exceptions \
-fno-rtti \
-fno-use-cxa-atexit \
-fno-common \
-fno-threadsafe-statics \
-finline-small-functions \
-findirect-inlining \
")

add_subdirectory(core)
add_subdirectory(sprinter)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} \
-fno-exceptions \
-fno-non-call-exceptions \
-fno-rtti \
-fno-use-cxa-atexit \
-fno-common \
-fno-threadsafe-statics \
-finline-small-functions \
-findirect-inlining \
-Os \
")

set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DBUILD_TYPE_DEBUG")

add_subdirectory(core)
add_subdirectory(sprinter)

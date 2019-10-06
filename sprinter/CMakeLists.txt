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
")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -os")

add_subdirectory(core)
add_subdirectory(sprinter)

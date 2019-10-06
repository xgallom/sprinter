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

if (${BUILD_TYPE_RELEASE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_TYPE_RELEASE")
endif ()

add_subdirectory(core)
add_subdirectory(sprinter)

cmake_minimum_required(VERSION 3.23)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

include_directories(.)

add_executable(RayTracer
        common.h
        xmath.h
        matrix.h
        geometry.h
        geometry.cpp
        geometry_unittest.cpp
        geometry_helper.h
        geometry_helper.cpp
        ray_tracer.h
        ray_tracer.cpp
        ray_tracer_unittest.cpp
        )
target_link_libraries(RayTracer GTest::gmock_main)

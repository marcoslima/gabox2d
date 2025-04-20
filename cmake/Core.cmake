target_link_libraries(${LIBRARY_NAME} GL)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Add coverage instrumentation only for debug builds
if (GABOX2D_ENABLE_COVERAGE)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            message(STATUS "Enabling code coverage for ${CMAKE_BUILD_TYPE} build")
            target_compile_options(GaBox2dLib PRIVATE --coverage -O0 -g)
            target_link_options(GaBox2dLib PRIVATE --coverage)
        endif ()
    else ()
        message(STATUS "Code coverage disabled for non-Debug build: ${CMAKE_BUILD_TYPE}")
    endif ()
endif ()
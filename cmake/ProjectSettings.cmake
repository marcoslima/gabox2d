# Define C++ standard to use
set(CMAKE_CXX_STANDARD 23)

if(GABOX2D_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Code coverage enabled for all targets")
        # These flags will apply to all targets
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        message(WARNING "Code coverage is only available with GCC or Clang")
    endif()
endif()

#########################################
# CATCH2
# Fetch Catch2 test framework:
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)
#########################################

# Enable testing with CTest
include(CTest)
include(Catch)

# Option to enable/disable testing
option(GABOX2D_BUILD_TESTS "Build the GaBox2d tests" ON)

if(GABOX2D_BUILD_TESTS)
  # Create the test executable
  add_executable(gabox2d_tests
    # Test files
    ${CMAKE_SOURCE_DIR}/tests/test_main.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_ga.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_car.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_random.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_doublex.cpp
  )

  # Link against the main project and Catch2
  target_link_libraries(gabox2d_tests
    PRIVATE
      GaBox2dLib  # Your main project library
      Catch2::Catch2WithMain
  )

  # Add include directories
  target_include_directories(gabox2d_tests
    PRIVATE
      ${CMAKE_SOURCE_DIR}/include
  )

  if(GABOX2D_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      message(STATUS "Code coverage enabled")
      target_compile_options(gabox2d_tests PRIVATE --coverage)
      target_link_options(gabox2d_tests PRIVATE --coverage)
    else()
      message(WARNING "Code coverage is only available with GCC or Clang")
    endif()
  endif()

  # Add compile definition for testing
  target_compile_definitions(gabox2d_tests PRIVATE TESTING)

  # Register tests with CTest
  catch_discover_tests(gabox2d_tests)

  # Add custom target for running tests with coverage
  if(GABOX2D_ENABLE_COVERAGE)
    message(STATUS "gcovr root: ${CMAKE_SOURCE_DIR}")
    add_custom_target(coverage
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        # Run tests first to generate coverage data
        COMMAND $<TARGET_FILE:gabox2d_tests>
        
        # Generate HTML report with gcovr
        COMMAND gcovr --root ${CMAKE_SOURCE_DIR}
                      --object-directory=${CMAKE_BINARY_DIR}
                      --exclude=".*imgui\.*"
                      --exclude=".*cdt\.*"
                      --exclude=".*box2d\.*"
                      --exclude=".*_deps\.*"
                      --html --html-details
                      -o ${CMAKE_BINARY_DIR}/coverage/index.html
        
        # Also output text summary to console
        COMMAND gcovr --root ${CMAKE_SOURCE_DIR}
                      --object-directory=${CMAKE_BINARY_DIR}
        
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating code coverage report..."
    )    
  endif()
endif()

# Add a custom target that only runs the unit tests (not the integration tests)
add_custom_target(unit-tests
  COMMAND ${CMAKE_CTEST_COMMAND} -L unit --output-on-failure
  DEPENDS gabox2d_tests
  COMMENT "Running unit tests"
)
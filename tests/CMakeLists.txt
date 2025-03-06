add_executable(
    unit_tests
    test_car.cpp
)

target_include_directories(unit_tests PRIVATE ../include)
include(../CMakeLists.txt)
target_link_libraries(
    unit_tests PRIVATE Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(unit_tests)

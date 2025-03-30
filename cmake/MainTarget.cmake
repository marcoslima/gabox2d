add_executable(GaBox2d src/GaBox2d.cpp
        ../include/vec2f_t.h)
target_link_libraries(GaBox2d PRIVATE GaBox2dLib)

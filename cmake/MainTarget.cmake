add_executable(GaBox2d src/GaBox2d.cpp
        ../include/IVec2f.h)
target_link_libraries(GaBox2d PRIVATE GaBox2dLib)

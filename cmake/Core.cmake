add_library(GaBox2dLib
        src/car.cpp
        src/CarDef.cpp
        src/CCronometro.cpp
        src/doublex.cpp
        src/EditorChaoDlg.cpp
        src/env.cpp
        src/EvolucaoDlg.cpp
        src/ga.cpp
        src/GaBox2dApp.cpp
        src/GaBox2dDoc.cpp
        src/GaBox2dView.cpp
        src/GaCar.cpp
        src/ga_car_helpers.cpp
        src/GaInfoDlg.cpp
        src/GaParamsDlg.cpp
        src/GrCar.cpp
        src/IdInfoDlg.cpp
        src/imgui_input_uint8.cpp
        src/lmFisica.cpp
        src/lmmath.cpp
        src/phys.cpp
        src/PhysCar.cpp
        src/WndPreviewChao.cpp
        src/WndShowCarro.cpp
        src/GaInfo.cpp
        src/World.cpp
        src/fn_ga.cpp
        src/main_menu.cpp
        src/sfml_primitives.cpp
        src/network/ga_server.cpp
        src/network/ipc_client.cpp
        src/triangulation.cpp
        src/random_genes_generator.cpp
        src/network/rs_codec.cpp

        include/assets.h
        include/car.h
        include/CarDef.h
        include/CCronometro.h
        include/CRandom.h
        include/doublex.h
        include/EditorChaoDlg.h
        include/env.h
        include/EvolucaoDlg.h
        include/ga_params.h
        include/ga.h
        include/GaBox2d.h
        include/GaBox2dApp.h
        include/GaBox2dDoc.h
        include/GaBox2dView.h
        include/GaCar.h
        include/ga_car_helpers.h
        include/GaInfoDlg.h
        include/GaParamsDlg.h
        include/GrCar.h
        include/IdInfoDlg.h
        include/imgui_input8.h
        include/lmfisica.h
        include/lmmath.h
        include/Pen.h
        include/phys.h
        include/PhysCar.h
        include/SolidBrush.h
        include/WndPreviewChao.h
        include/WndShowCarro.h
        include/World.h
        include/GaInfo.h
)

target_include_directories("GaBox2dLib" PUBLIC ${SFML_INCLUDE_DIRS})
target_include_directories("GaBox2dLib" PUBLIC ${MSGPACK_PATH}/msgpack/include)
target_include_directories("GaBox2dLib" PUBLIC ${REEDSOLOMON_PATH}/include)
target_link_directories("GaBox2dLib" PUBLIC ${REEDSOLOMON_PATH}/build)
target_link_libraries("GaBox2dLib" imgui imgui-sfml sfml-graphics sfml-audio GL box2d ReedSolomon ${UUID_LIBRARIES})

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
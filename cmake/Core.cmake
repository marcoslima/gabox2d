add_library(GaBox2dLib
    src/car.cpp 
    src/CarDef.cpp 
    src/CCronometro.cpp 
    src/devutils.cpp 
    src/doublex.cpp 
    src/EditorChaoDlg.cpp 
    src/env.cpp 
    src/EvolucaoDlg.cpp 
    src/ga.cpp 
    src/GaBox2dApp.cpp 
    src/GaBox2dDoc.cpp 
    src/GaBox2dView.cpp 
    src/GaCar.cpp 
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

    include/assets.h
    include/car.h
    include/CarDef.h
    include/CCronometro.h
    include/CRandom.h
    include/devutils.h
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
    include/random_provider.h
)

target_include_directories("GaBox2dLib" PUBLIC ${SFML_INCLUDE_DIRS})
target_link_libraries("GaBox2dLib" imgui imgui-sfml sfml-graphics sfml-audio GL box2dd)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

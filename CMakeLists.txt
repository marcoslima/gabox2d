cmake_minimum_required(VERSION 3.14)

# Define project name and version
project("GaBox2d" VERSION 2.0)

# Define C++ standard to use
set(CMAKE_CXX_STANDARD 23)

# Find SFML library
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Define the path where the ImGui library will be cloned
set(IMGUI_PATH ${CMAKE_CURRENT_BINARY_DIR}/imgui)
set(IMGUI_SFML_PATH ${IMGUI_PATH}/imgui-sfml)

# Clone the ImGui library from its GitHub repository
IF (NOT EXISTS "${IMGUI_PATH}")
    execute_process(
            COMMAND git clone https://github.com/ocornut/imgui.git ${IMGUI_PATH}
    )
ELSE ()
    execute_process(
            COMMAND pushd ${IMGUI_PATH} && git pull && popd
    )
ENDIF ()

# Clone the ImGui-SFML library from its GitHub repository
IF (EXISTS "${IMGUI_SFML_PATH}")
    execute_process(
            COMMAND rm -rf "${IMGUI_SFML_PATH}"
    )
ENDIF ()
execute_process(
        COMMAND git clone https://github.com/eliasdaler/imgui-sfml.git "${IMGUI_SFML_PATH}"
)
execute_process(
        COMMAND git checkout 2.6.x
        WORKING_DIRECTORY "${IMGUI_SFML_PATH}"
)


# Add the ImGui and ImGui-SFML libraries to the project
add_library(imgui STATIC
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp
        ${IMGUI_PATH}/imgui_tables.cpp
)

add_library(imgui-sfml STATIC
        ${IMGUI_PATH}/imgui-sfml/imgui-SFML.cpp
)


# Add include directories for the ImGui and SFML libraries
target_include_directories(imgui PUBLIC ${IMGUI_PATH})
target_include_directories(imgui-sfml PUBLIC ${IMGUI_PATH}/imgui-sfml)
target_include_directories(imgui-sfml PUBLIC ${IMGUI_PATH})

#########################################################
## BOX2D
# Define the path where the Box2D library will be cloned
set(BOX2D_PATH ${CMAKE_CURRENT_BINARY_DIR}/box2d)
message(STATUS ">>>>> BOX2D_PATH: ${BOX2D_PATH}")


# Clone the Box2D library from its GitHub repository
IF (NOT EXISTS "${BOX2D_PATH}")
    message(STATUS ">>>>> Cloning Box2D library into ${BOX2D_PATH}")
    execute_process(
            COMMAND git clone https://github.com/erincatto/box2d.git ${BOX2D_PATH}
    )
ELSE ()
    message(STATUS ">>>>> Pulling Box2D library at ${BOX2D_PATH}")
    execute_process(
            COMMAND git pull
            WORKING_DIRECTORY ${BOX2D_PATH}
    )
ENDIF ()

# Build the Box2D library
IF (NOT EXISTS "${BOX2D_PATH}/build")
    message(STATUS ">>>>> Creating Box2D build directory")
    execute_process(
            COMMAND mkdir ${BOX2D_PATH}/build
    )
ENDIF ()
IF (NOT EXISTS "${BOX2D_PATH}/build/bin/libbox2d.a")
    message(STATUS ">>>>> Building Box2D library")
    execute_process(
            WORKING_DIRECTORY ${BOX2D_PATH}/build
            COMMAND cmake .. -DBOX2D_BUILD_DOCS=OFF -DCMAKE_BUILD_TYPE=Debug
    )
    execute_process(
            WORKING_DIRECTORY ${BOX2D_PATH}/build
            COMMAND cmake --build . -j20
    )
ENDIF ()

# Add include directories for the Box2D library
message(STATUS ">>>>> Adding Box2D include directories (${BOX2D_PATH}/include)")
target_include_directories(imgui PUBLIC ${BOX2D_PATH}/include)

# Add library directory for the Box2D library
message(STATUS ">>>>> Adding Box2D library directory (${BOX2D_PATH}/build/src)")
link_directories(${BOX2D_PATH}/build/src)
###########################################################

###########################################################
## CDT Library

# Define the path where the CDT library will be cloned
set(CDT_PATH ${CMAKE_CURRENT_BINARY_DIR}/cdt)

# Clone the CDT library from its GitHub repository
IF (NOT EXISTS "${CDT_PATH}")
    execute_process(
            COMMAND git clone https://github.com/artem-ogre/CDT.git ${CDT_PATH}

    )
ELSE ()
    execute_process(
            COMMAND git pull
            WORKING_DIRECTORY ${CDT_PATH}
    )
ENDIF ()

# Build the CDT library
execute_process(
        COMMAND cmake --build -DCDT_USE_AS_COMPILED_LIBRARY=OFF
        WORKING_DIRECTORY ${CDT_PATH}/CDT/build
)

# Add include directories for the CDT library
target_include_directories(imgui PUBLIC ${CDT_PATH}/CDT/include)

###########################################################

add_executable(GaBox2d
        GaBox2d.cpp
        GaParamsDlg.cpp
        EditorChaoDlg.cpp
        WndPreviewChao.cpp
        env.cpp
        imgui_input_uint8.cpp
        GaBox2dDoc.cpp
        devutils.cpp
        GaBox2dView.cpp
        GaCar.cpp
        PhysCar.h
        PhysCar.cpp
        CRandom.h
        car.cpp
        GrCar.cpp
        CarDef.cpp
        phys.cpp
        ga_params.h
        CCronometro.cpp
        CCronometro.h
        ga.cpp
        ga.h
)

# Add definition globally
add_definitions(-UCDT_USE_AS_COMPILED_LIBRARY)

target_include_directories("GaBox2d" PUBLIC ${SFML_INCLUDE_DIRS})
target_link_libraries("GaBox2d" imgui imgui-sfml sfml-graphics sfml-audio GL box2dd)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

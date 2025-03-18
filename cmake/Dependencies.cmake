include(FetchContent)

##########################################
# SFML
##########################################
# Find SFML library
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Define the path where the ImGui library will be cloned
set(IMGUI_PATH ${CMAKE_CURRENT_BINARY_DIR}/imgui)
set(IMGUI_SFML_PATH ${IMGUI_PATH}/imgui-sfml)
##########################################



#########################################################
# IMGUI
#########################################################
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
#########################################################



#####################################################################################
# IMGUI-SFML
#####################################################################################
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
#####################################################################################



#########################################################
# IMGUI
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
set(CDT_USE_AS_COMPILED_LIBRARY OFF)

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
    execute_process(
            COMMAND git checkout .
            WORKING_DIRECTORY ${CDT_PATH}
    )
ENDIF ()

###########################################################
## UUID Library
find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)

include_directories(${UUID_INCLUDE_DIRS})
link_directories(${UUID_LIBRARY_DIRS})
###########################################################

# MONKEY PATCH para resolver o problema do "redefinition etc":
# Add #pragma once to CDT.hpp
execute_process(
    COMMAND bash -c "sed -i '1i#pragma once' ${CDT_PATH}/CDT/include/CDT.hpp"
)

# Build the CDT library
execute_process(
        COMMAND cmake --build -DCDT_USE_AS_COMPILED_LIBRARY=${CDT_USE_AS_COMPILED_LIBRARY}
        WORKING_DIRECTORY ${CDT_PATH}/CDT/build
)

# Add include directories for the CDT library
target_include_directories(imgui PUBLIC ${CDT_PATH}/CDT/include include)

###########################################################


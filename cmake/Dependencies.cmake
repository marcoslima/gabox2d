include(FetchContent)

# Ascart's feitos no site abaixo em 5 de abril de 2025.
# https://patorjk.com/software/taag/#p=display&f=BlurVision%20ASCII&t=ASCART

####################################################################
#  ░▒▓███████▓▒░▒▓████████▓▒░▒▓██████████████▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
#  ░▒▓██████▓▒░░▒▓██████▓▒░ ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
#        ░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
#        ░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓███████▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░
####################################################################
# Find SFML library
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Define the path where the ImGui library will be cloned
set(IMGUI_PATH ${CMAKE_CURRENT_BINARY_DIR}/imgui)
set(IMGUI_SFML_PATH ${IMGUI_PATH}/imgui-sfml)
##########################################



##############################################################
# ░▒▓█▓▒░▒▓██████████████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░
##############################################################
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



#################################################################################################################################
#░▒▓█▓▒░▒▓██████████████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░       ░▒▓███████▓▒░▒▓████████▓▒░▒▓██████████████▓▒░░▒▓█▓▒░
#░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
#░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
#░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░ ████  ░▒▓██████▓▒░░▒▓██████▓▒░ ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
#░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
#░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
#░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░      ░▒▓███████▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░

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



#####################################################################
# ░▒▓█▓▒░▒▓██████████████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
# ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░
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


######################################################################
# ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓███████▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░
# Define the path where the Box2D library will be cloned
set(BOX2D_PATH ${CMAKE_CURRENT_BINARY_DIR}/box2d)
set(BOX2D_VERSION "28adacf82377d4113f2ed00586141463244b9d10")
message(STATUS ">>>>> BOX2D_PATH: ${BOX2D_PATH}")
message(STATUS ">>>>> BOX2D_VERSION: ${BOX2D_VERSION}")


# Clone the Box2D library from its GitHub repository
IF (NOT EXISTS "${BOX2D_PATH}")
    message(STATUS ">>>>> Cloning Box2D library into ${BOX2D_PATH}")
    execute_process(
            COMMAND git clone https://github.com/erincatto/box2d.git ${BOX2D_PATH}
    )
ELSE ()
    message(STATUS ">>>>> Pulling Box2D library at ${BOX2D_PATH}")
    execute_process(
            COMMAND git fetch
            WORKING_DIRECTORY ${BOX2D_PATH}
    )
ENDIF ()
# Let's use the correct version of box2d:
execute_process(
        COMMAND git checkout ${BOX2D_VERSION}
        WORKING_DIRECTORY ${BOX2D_PATH}
)
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
            COMMAND cmake .. -DCMAKE_BUILD_TYPE=Release -DBOX2D_AVX2=ON -DBOX2D_UNIT_TESTS=OFF -DBOX2D_ENABLE_SIMD=ON -DBOX2D_SAMPLES=OFF -DBOX2D_UNIT_TESTS=OFF
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
# ░▒▓██████▓▒░░▒▓███████▓▒░▒▓████████▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░
# ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░
# ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░
# ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░
# ░▒▓██████▓▒░░▒▓███████▓▒░  ░▒▓█▓▒░

# Define the path where the CDT library will be cloned
set(CDT_PATH ${CMAKE_CURRENT_BINARY_DIR}/cdt)
set(CDT_USE_AS_COMPILED_LIBRARY OFF)

# Clone the CDT library from its GitHub repository
IF (NOT EXISTS "${CDT_PATH}")
    execute_process(
            COMMAND git clone https://github.com/marcoslima/CDT.git ${CDT_PATH}
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

# Build the CDT library
execute_process(
        COMMAND cmake --build -DCDT_USE_AS_COMPILED_LIBRARY=${CDT_USE_AS_COMPILED_LIBRARY}
        WORKING_DIRECTORY ${CDT_PATH}/CDT/build
)

# Add include directories for the CDT library
target_include_directories(imgui PUBLIC ${CDT_PATH}/CDT/include include)
###########################################################


###########################################################
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓███████▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░▒▓███████▓▒░
find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)

include_directories(${UUID_INCLUDE_DIRS})
link_directories(${UUID_LIBRARY_DIRS})
###########################################################


#######################################################################################################
# ░▒▓██████████████▓▒░ ░▒▓███████▓▒░░▒▓██████▓▒░░▒▓███████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒▒▓███▓▒░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░      ░▒▓███████▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░
# https://github.com/ar90n/msgpack11
# Define the path where the msgpack library will be cloned
set(MSGPACK_PATH ${CMAKE_CURRENT_BINARY_DIR}/msgpack)

# Clone the msgpack library from its GitHub repository
IF (NOT EXISTS "${MSGPACK_PATH}")
    message(STATUS ">>>>> Cloning msgpack library into ${MSGPACK_PATH}")
    execute_process(
            COMMAND git clone https://github.com/ar90n/msgpack11.git ${MSGPACK_PATH}
    )
ELSE ()
    message(STATUS ">>>>> Pulling msgpack library at ${MSGPACK_PATH}")
    execute_process(
            COMMAND git pull
            WORKING_DIRECTORY ${MSGPACK_PATH}
    )
ENDIF ()
# Build the msgpack library
IF (NOT EXISTS "${MSGPACK_PATH}/build")
    message(STATUS ">>>>> Creating Msgpack build directory")
    execute_process(
            COMMAND mkdir ${MSGPACK_PATH}/build
    )
ENDIF ()

message(STATUS ">>>>> Building Msgpack library")
execute_process(
        WORKING_DIRECTORY ${MSGPACK_PATH}/build
        COMMAND cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DMSGPACK11_BUILD_TESTS=OFF
)
execute_process(
        WORKING_DIRECTORY ${MSGPACK_PATH}/build
        COMMAND cmake --build . -j20
)

#########################################################################################################################################################
# ░▒▓███████▓▒░░▒▓████████▓▒░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓███████▓▒░░▒▓██████▓▒░░▒▓█▓▒░      ░▒▓██████▓▒░░▒▓██████████████▓▒░ ░▒▓██████▓▒░░▒▓███████▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓███████▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░     ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
# ░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓████████▓▒░▒▓███████▓▒░░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓████████▓▒░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░

set(REEDSOLOMON_GIT_PATH "https://github.com/marcoslima/ReedSolomon.git")
set(REEDSOLOMON_CHECKOUT "master")
set(REEDSOLOMON_PATH ${CMAKE_CURRENT_BINARY_DIR}/ReedSolomon)

# Clone the ReedSolomon library from its GitHub repository
IF (NOT EXISTS "${REEDSOLOMON_PATH}")
    message(STATUS ">>>>> Cloning ReedSolomon library into ${REEDSOLOMON_PATH}")
    execute_process(
            COMMAND git clone ${REEDSOLOMON_GIT_PATH} ${REEDSOLOMON_PATH}
    )
    execute_process(
            COMMAND git checkout ${REEDSOLOMON_CHECKOUT}
            WORKING_DIRECTORY ${REEDSOLOMON_PATH}
    )
ELSE ()
    message(STATUS ">>>>> Pulling ReedSolomon library at ${REEDSOLOMON_PATH}")
    execute_process(
            COMMAND git pull ${REEDSOLOMON_GIT_PATH}
            WORKING_DIRECTORY ${REEDSOLOMON_PATH}
    )
ENDIF ()

# Build the ReedSolomon library
IF (NOT EXISTS "${REEDSOLOMON_PATH}/build")
    message(STATUS ">>>>> Creating ReedSolomon build directory")
    execute_process(
            COMMAND mkdir ${REEDSOLOMON_PATH}/build
    )
ENDIF ()

message(STATUS ">>>>> Building ReedSolomon library")
execute_process(
        WORKING_DIRECTORY ${REEDSOLOMON_PATH}/build
        COMMAND cmake .. -DCMAKE_BUILD_TYPE=Release
)
execute_process(
        WORKING_DIRECTORY ${REEDSOLOMON_PATH}/build
        COMMAND cmake --build . -j20
)

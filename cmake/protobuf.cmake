file(GLOB_RECURSE PROTO_SOURCES CONFIGURE_DEPENDS
        "genproto/**/*.cc"
)

add_library(protobuf
        STATIC
        ${PROTO_SOURCES}
)
target_include_directories(protobuf
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/genproto
)
target_include_directories(${LIBRARY_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/genproto
)
target_link_libraries(${LIBRARY_NAME} protobuf)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(protobuf ${Protobuf_LIBRARIES})
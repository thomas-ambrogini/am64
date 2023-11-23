file(GLOB HEADER_UTILS_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/include/utils/*.hpp")
file(GLOB SOURCES_UTILS_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/src/utils/*.cpp")

add_library(utils_library ${SOURCES_UTILS_LIST} ${HEADER_UTILS_LIST})

target_include_directories(utils_library PUBLIC "${embedded_kafka_SOURCE_DIR}/include/utils")
target_link_libraries(utils_library PUBLIC json_library)
target_link_libraries(utils_library PUBLIC ti_rpmsg_char_library)


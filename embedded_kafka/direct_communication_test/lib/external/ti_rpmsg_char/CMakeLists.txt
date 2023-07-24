file(GLOB HEADER_TI_RPMSG_CHAR_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/external/ti_rpmsg_char/include/*.h")
file(GLOB SOURCES_TI_RPMSG_CHAR_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/external/ti_rpmsg_char/*.c")

add_library(ti_rpmsg_char_library ${SOURCES_TI_RPMSG_CHAR_LIST} ${HEADER_TI_RPMSG_CHAR_LIST})

target_include_directories(ti_rpmsg_char_library PUBLIC "${embedded_kafka_SOURCE_DIR}/external/ti_rpmsg_char/include")

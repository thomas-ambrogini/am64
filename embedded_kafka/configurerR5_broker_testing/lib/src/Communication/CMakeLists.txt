file(GLOB HEADER_COMMUNICATION_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/include/Communication/*.hpp")
file(GLOB SOURCES_COMMUNICATION_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/src/Communication/*.cpp")

add_library(communication_library ${SOURCES_COMMUNICATION_LIST} ${HEADER_COMMUNICATION_LIST})

target_include_directories(communication_library PUBLIC "${embedded_kafka_SOURCE_DIR}/include/Communication")
target_link_libraries(communication_library PUBLIC domain_library)
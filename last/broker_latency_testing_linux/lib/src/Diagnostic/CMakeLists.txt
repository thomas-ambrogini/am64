file(GLOB HEADER_DIAGNOSTIC_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/include/Diagnostic/*.hpp")
file(GLOB SOURCES_DIAGNOSTIC_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/src/Diagnostic/*.cpp")

add_library(diagnostic_library ${SOURCES_DIAGNOSTIC_LIST} ${HEADER_DIAGNOSTIC_LIST})

target_include_directories(diagnostic_library PUBLIC "${embedded_kafka_SOURCE_DIR}/include/Diagnostic")
target_link_libraries(diagnostic_library PUBLIC topicmanager_library)

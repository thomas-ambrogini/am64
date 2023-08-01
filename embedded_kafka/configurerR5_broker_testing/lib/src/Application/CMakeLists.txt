file(GLOB HEADER_APPLICATION_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/include/Application/*.hpp")
file(GLOB SOURCES_APPLICATION_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/src/Application/*.cpp")

add_library(application_library ${SOURCES_APPLICATION_LIST} ${HEADER_APPLICATION_LIST})

target_include_directories(application_library PUBLIC "${embedded_kafka_SOURCE_DIR}/include/Application")
target_link_libraries(application_library PUBLIC diagnostic_library)

file(GLOB HEADER_DOMAIN_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/include/Domain/*.hpp")
file(GLOB SOURCES_DOMAIN_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/src/Domain/*.cpp")

add_library(domain_library ${SOURCES_DOMAIN_LIST} ${HEADER_DOMAIN_LIST})

target_include_directories(domain_library PUBLIC "${embedded_kafka_SOURCE_DIR}/include/Domain")
target_link_libraries(domain_library PUBLIC utils_library)
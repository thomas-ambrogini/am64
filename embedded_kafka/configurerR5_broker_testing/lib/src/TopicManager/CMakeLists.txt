file(GLOB HEADER_TOPICMANAGER_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/include/TopicManager/*.hpp")
file(GLOB SOURCES_TOPICMANAGER_LIST CONFIGURE_DEPENDS "${embedded_kafka_SOURCE_DIR}/src/TopicManager/*.cpp")

add_library(topicmanager_library ${SOURCES_TOPICMANAGER_LIST} ${HEADER_TOPICMANAGER_LIST})

target_include_directories(topicmanager_library PUBLIC "${embedded_kafka_SOURCE_DIR}/include/TopicManager")
target_link_libraries(topicmanager_library PUBLIC communication_library)


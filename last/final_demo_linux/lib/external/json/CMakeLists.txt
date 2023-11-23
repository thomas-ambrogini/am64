add_library(json_library INTERFACE)

target_include_directories(json_library INTERFACE "${embedded_kafka_SOURCE_DIR}/external/json/")
add_library(logger_static STATIC ./source/FileLogger.cpp
		./source/SocketLogger.cpp)
add_library(logger_shared SHARED ./source/FileLogger.cpp
		./source/SocketLogger.cpp)

target_include_directories(logger_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(logger_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(logger_static PROPERTIES OUTPUT_NAME "logger")
set_target_properties(logger_shared PROPERTIES OUTPUT_NAME "logger")
add_executable(logger main.cpp
		source/LoggerClient.cpp source/Config.cpp)
target_link_libraries(logger PRIVATE logger_static)
#target_link_libraries(logger PRIVATE logger_shared)

target_include_directories(logger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_command(TARGET logger POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_SOURCE_DIR}/logger.conf
	$<TARGET_FILE_DIR:logger>/logger.conf
	COMMENT "Copying logger.conf to build directory")
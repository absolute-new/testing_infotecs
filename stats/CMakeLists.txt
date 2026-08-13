add_executable(stats main.cpp
	source/StatsServer.cpp
	source/Timer.cpp)

target_link_libraries(stats PRIVATE logger_static)
#target_link_libraries(stats PRIVATE logger_shared)

target_include_directories(stats PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
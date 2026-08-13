add_executable(tests
	test_FileLogger.cpp
	test_SocketLogger.cpp
	${CMAKE_SOURCE_DIR}/stats/source/StatsServer.cpp
	${CMAKE_SOURCE_DIR}/stats/source/Timer.cpp)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/stats/include
)

add_test(NAME FileLoggerTest COMMAND tests --gtest_filter=FileLoggerTest.*)
add_test(NAME SocketLoggerTest COMMAND tests --gtest_filter=SocketLoggerTest.*:IntegrationTest.*)


set_target_properties(tests PROPERTIES OUTPUT_NAME "tests")
target_link_libraries(tests PRIVATE logger_static gtest gtest_main)
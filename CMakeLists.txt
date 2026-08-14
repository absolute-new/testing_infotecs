cmake_minimum_required(VERSION 3.1...3.5)
project(LoggerProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

enable_testing()

add_subdirectory(logger)
add_subdirectory(lib)
add_subdirectory(stats)
add_subdirectory(tests)


find_program(LCOV lcov)
if (LCOV)
	add_custom_target(gcov_report
		COMMAND ${CMAKE_COMMAND} -E echo "Running test for coverage.."
		COMMAND ${CMAKE_CTEST_COMMAND} --verbose
		COMMAND lcov --ignore-errors mismatch --capture --directory . --output-file coverage.info
		COMMAND lcov --remove coverage.info '/usr/*' '/tests/*' '/include/*' --output coverage.info
		COMMAND genhtml coverage.info --output-directory htmlcov
	)
endif()

find_package(Doxygen)
if (DOXYGEN_FOUND)
	add_custom_target(docs
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generate Doxygen documentation")
endif()

find_program(CLANG_FORMAT clang-format)
if (CLANG_FORMAT)
	add_custom_target(format
		COMMAND find ${CMAKE_SOURCE_DIR} -path '${CMAKE_BINARY_DIR}' -prune -o 
			-name "*.cpp" -exec clang-format -i --style=file:${CMAKE_SOURCE_DIR}/linters/.clang-format {} \\\;
		COMMENT "Formatting all .cpp with clang-format (config: linters/.clang-format)"
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

	add_custom_target(check-format
		COMMAND find ${CMAKE_SOURCE_DIR} -path '${CMAKE_BINARY_DIR}' -prune -o 
			-name '*.cpp' -exec clang-format -n --style=file:${CMAKE_SOURCE_DIR}/linters/.clang-format {} \\\;
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_custom_target(fclean
	COMMAND find ${CMAKE_BINARY_DIR} -mindepth 1 -delete
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(dist
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/dist
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/lib ${CMAKE_BINARY_DIR}/dist/lib
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/stats ${CMAKE_BINARY_DIR}/dist/stats
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/logger ${CMAKE_BINARY_DIR}/dist/logger
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/tests ${CMAKE_BINARY_DIR}/dist/tests
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/CMakeLists.txt ${CMAKE_BINARY_DIR}/dist
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/dist
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/logger.conf ${CMAKE_BINARY_DIR}/dist
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/README.md ${CMAKE_BINARY_DIR}/dist
	COMMAND tar -czf ${CMAKE_BINARY_DIR}/logger-project.tar.gz -C ${CMAKE_BINARY_DIR}/dist .
	COMMAND ${CMAKE_COMMAND} -E echo "Distribution created: ${CMAKE_BINARY_DIR}/logger-project.tar")



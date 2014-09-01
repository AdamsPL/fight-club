cmake_minimum_required(VERSION 2.8)

enable_testing()

find_package(GTest REQUIRED)
find_package(Qt4 COMPONENTS QtCore REQUIRED)

include(../CMakeLists.common.txt)
include(${QT_USE_FILE})

include_directories(${GTEST_INCLUDE_DIR} ../engine/)

set(NAME "engine_ut")
set(SRC
	../engine/Config.cc
	testGameEnvironment.cc
	testConfig.cc
)

set(DEPLOYABLE
	example.cfg
)

add_executable(${NAME} ${SRC})
target_link_libraries(${NAME} ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} ${QT_LIBRARIES})

add_test(
	NAME ${NAME}
	COMMAND ${NAME}
)

add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy example.cfg ${CMAKE_CURRENT_BINARY_DIR})

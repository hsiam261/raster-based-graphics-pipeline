set(UNIT_TESTS ${CMAKE_PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)
set(SOURCES ${TEST_SOURCES})

add_executable(${UNIT_TESTS} ${TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../src/geometry.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/camera.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/shapes.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/canvas.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/frame_buffer.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/../src/color.cpp) 
add_test(NAME ${UNIT_TESTS} COMMAND ${UNIT_TESTS})

target_include_directories(${UNIT_TESTS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)


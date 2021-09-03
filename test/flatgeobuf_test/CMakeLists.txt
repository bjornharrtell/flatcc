include(CTest)

set(INC_DIR "${PROJECT_SOURCE_DIR}/include")
set(GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(FBS_DIR "${PROJECT_SOURCE_DIR}/test/flatgeobuf_test")

include_directories("${GEN_DIR}" "${INC_DIR}")

add_custom_target(gen_flatgeobuf_test ALL) 
add_custom_command (
    TARGET gen_flatgeobuf_test
    COMMAND cmake -E make_directory "${GEN_DIR}"
    COMMAND flatcc_cli -a -o "${GEN_DIR}" "${FBS_DIR}/feature.fbs"
    DEPENDS flatcc_cli "${FBS_DIR}/header.fbs"
)
add_executable(flatgeobuf_test flatgeobuf_test.c)
add_dependencies(flatgeobuf_test gen_flatgeobuf_test)
target_link_libraries(flatgeobuf_test flatccrt)

add_test(flatgeobuf_test flatgeobuf_test${CMAKE_EXECUTABLE_SUFFIX})

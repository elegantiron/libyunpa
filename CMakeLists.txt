cmake_minimum_required(VERSION 3.30)
project(
    libyunpa
    VERSION 0.0.1
    LANGUAGES CXX
)
add_library(${PROJECT_NAME} SHARED)
target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_20
)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    pegtl
)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    PREFIX ""
)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    -Werror -Wall -Wextra -Weffc++
    -pedantic-errors -Wconversion
    -Wsign-conversion
)
else()
target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    -O3 -DNDEBUG
)
endif()
set(
    RELATIVE_MODULES
    Parsing.cppm
    Time.cppm
)
foreach(source_file IN LISTS RELATIVE_MODULES)
list(APPEND ABSOLUTE_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/modules/libyunpa/${source_file})
endforeach()
list(APPEND ABSOLUTE_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/modules/libyunpa.cppm)
target_sources(
    ${PROJECT_NAME}
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
    ${ABSOLUTE_MODULES}
)
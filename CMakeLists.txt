cmake_minimum_required(VERSION 3.30)
project(
    libyunpa
    VERSION 0.0.1
    LANGUAGES CXX
)
add_library(${PROJECT_NAME})
target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_26
)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    pegtl
    PUBLIC
    ftxui::modules
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
add_subdirectory(modules)
cmake_minimum_required(VERSION 3.1)

project(JSON_helper
        VERSION 0.2.0
        LANGUAGES CXX
    )

SET(CMAKE_CXX_STANDARD 17)


set(JSON_helper_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(JSON_helper_SRC_DIR "${PROJECT_SOURCE_DIR}/src")

add_library(JSON_helper INTERFACE)
target_include_directories(JSON_helper INTERFACE "${JSON_helper_INCLUDE_DIR}")
target_sources(JSON_helper INTERFACE "${JSON_helper_SRC_DIR}/json_helper.cpp")
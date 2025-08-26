cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "c_result")
set(PROJECT_VERSION 0.2)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang+)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES C)
add_library(${PROJECT_NAME} INTERFACE
        include/c_result.h)
add_executable(${PROJECT_NAME}_test
        test/c_result.c)

# Link test executable to library to inherit include directories
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Installation rules
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Create CMake package config
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
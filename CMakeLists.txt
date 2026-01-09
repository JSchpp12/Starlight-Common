cmake_minimum_required(VERSION 3.20)

project(starlight_common
    VERSION 1.0.0
    DESCRIPTION "Common library for use across Starlight projects"
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(absl CONFIG REQUIRED)

# ---- Target ----
add_library(${PROJECT_NAME} INTERFACE)

set(${PROJECT_NAME}_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/FrameTracker.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/IRenderDevice.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/VulkanHelpers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/IEvent.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/Handle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/HandleTypeRegistry.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/Renderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/IDeviceContext.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/EventBus.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/SubscriberCallbackInfo.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/helper/CastHelpers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/star_common/helper/FileHelpers.hpp
)
foreach(header ${${PROJECT_NAME}_HEADERS})
    target_sources(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${header}>)
endforeach()

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:${absl_INCLUDE_DIR}>
)

target_link_libraries(${PROJECT_NAME} INTERFACE absl::flat_hash_map)

# Provide a namespaced ALIAS for in-tree consumers
add_library(Starlight::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ---- Package config (install tree only) ----
set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install headers
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install the interface target and export it
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
)

# Install the exported targets file (this is the one that consumers include)
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE Starlight::
    DESTINATION ${CONFIG_INSTALL_DIR}
)

# Install the package config
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CONFIG_INSTALL_DIR}
)

# If you want to support the user package registry for dev:
# export(PACKAGE ${PROJECT_NAME})

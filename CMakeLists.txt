cmake_minimum_required(VERSION 3.16)
Project(OCL-VideoProcessingLibrary)
add_subdirectory(lib/FFMPEG)


set(PROJECT OCL-VideoProcessingLibrary)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
file(GLOB PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
add_library(${PROJECT} STATIC ${PROJECT_SOURCES})
target_link_libraries(${PROJECT} FFMPEG)

set_target_properties(${PROJECT} PROPERTIES PUBLIC_HEADER "${PROJECT_HEADERS}")

include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT}
        EXPORT ${PROJECT}Targets
        LIBRARY DESTINATION lib/
        PUBLIC_HEADER DESTINATION include/${PROJECT}/
)

install(
  EXPORT ${PROJECT}Targets
  DESTINATION lib/cmake/${PROJECT}
  FILE ${PROJECT}Targets.cmake
  )

set(INCLUDE_INSTALL_DIR include/ CACHE STRING "" )
set(LIB_INSTALL_DIR lib/ CACHE STRING "" )
set(SYSCONFIG_INSTALL_DIR etc/${PROJECT}/ CACHE STRING "" )

include(CMakePackageConfigHelpers)

configure_package_config_file(Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}Config.cmake
  INSTALL_DESTINATION ${LIB_INSTALL_DIR}/${PROJECT}/cmake
  PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}ConfigVersion.cmake
  VERSION 1.0
  COMPATIBILITY SameMajorVersion )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}ConfigVersion.cmake
        DESTINATION lib/cmake/${PROJECT} )

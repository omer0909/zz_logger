cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wshadow -W -O2 -flto")

project(zz_logger)

# Packaging support
set(CPACK_PACKAGE_VENDOR "Vendor name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Some summary")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    /.git
    /dist
    /.*build.*
    /\\\\.DS_Store
)

include(CPack)

# compile
set(PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/src/Logger.cpp)
set(PROJECT_INCLUDES ${CMAKE_SOURCE_DIR}/include)
include_directories()

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDES})

#install
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
        DESTINATION include
)

include(CMakePackageConfigHelpers)
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    DESTINATION cmake COMPONENT dev)

add_subdirectory(test)

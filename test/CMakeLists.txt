cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(test)

# set(CMAKE_PREFIX_PATH "/home/omer/Desktop/buraya/usr/local/" ${CMAKE_PREFIX_PATH})
# find_package(zz_loggger REQUIRED)

set(SOURCES test.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/libzz_loggger.a)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDES})

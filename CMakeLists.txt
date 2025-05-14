cmake_minimum_required(VERSION 3.28.3)
project(PrettyPrint)
set(CMAKE_CXX_STANDARD 23)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CPP_HEADERS "Source/Public/PrettyPrint/PrettyPrint.h")
set(CPP_SOURCES "Source/Private/PrettyPrint.cpp")

source_group("Public" FILES ${CPP_HEADERS})
source_group("Private" FILES ${CPP_SOURCES})
source_group("Tests" FILES "Source/Tests/Test.cpp")

add_library(${PROJECT_NAME} STATIC ${CPP_HEADERS} ${CPP_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "Source/Public")

add_executable(${PROJECT_NAME}Test "Source/Tests/Test.cpp")
target_include_directories(${PROJECT_NAME}Test PRIVATE "Source/Public")
target_link_libraries(${PROJECT_NAME}Test ${PROJECT_NAME})
add_dependencies(${PROJECT_NAME}Test ${PROJECT_NAME})
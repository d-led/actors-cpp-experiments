cmake_minimum_required(VERSION 3.0.0)
project(actor-experiments-cpp VERSION 0.1.0)

include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

include(CTest)
enable_testing()

find_package(sobjectizer CONFIG REQUIRED)
add_executable(actor-experiments-cpp src/main.cpp)


target_link_libraries(actor-experiments-cpp sobjectizer::StaticLib)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.16)

project(project-web VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/qwindowkit)
add_subdirectory(src)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

qt_standard_project_setup()

qt_add_executable(project-web 
    ${SOURCES}
)

target_link_libraries(project-web PRIVATE Qt6::Widgets Qt6::Core)

set_target_properties(project-web PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
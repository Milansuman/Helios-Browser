cmake_minimum_required(VERSION 3.16)

project(project-web VERSION 0.2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:\\Qt\\6.7.2\\msvc2019_64")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui WebEngineWidgets QuickWidgets)

if(LINUX)
    find_package(KF6WindowSystem)
endif()

qt_standard_project_setup()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

qt_add_executable(project-web
    ${SOURCES}
    resources.qrc
)

target_link_libraries(project-web PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::WebEngineWidgets Qt6::QuickWidgets)

if(LINUX)
    target_link_libraries(project-web PRIVATE KF6::WindowSystem)
endif()

set_target_properties(project-web PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(project-web)
endif()
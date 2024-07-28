add_subdirectory(misc)
add_subdirectory(components)
add_subdirectory(windows)
add_subdirectory(api)

set(SOURCES 
    ${SOURCES}
    src/main.cpp
    PARENT_SCOPE
)
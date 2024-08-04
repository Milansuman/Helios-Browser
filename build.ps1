param (
    [string]$config
)

cmake -DQt6_DIR=C:/Qt/6.7.2/msvc2019_64/lib/cmake/Qt6 -DQT_ADDITIONAL_PACKAGES_PREFIX_PATH=C:C:/Qt/6.7.2/msvc2019_64/ ..
cmake --build . --config $config
& "./${config}/project-web.exe"
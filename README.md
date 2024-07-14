# Project Web
Project Web (name change pending) is a web browser focused on the needs of developers, designers and power users in general.

![Our design](screenshots/design.png)

# Build Steps

Download and install the opensource version of qt. [download qt](https://www.qt.io/download-open-source)

Ensure QtWebEngine, WebChannel, WebSockets and Qt Positioning is installed. This project may use even more additional libraries, so it's best to install them all.

On windows, it is essential to pick the msvc compiler(This is available in the qt installer tool and maintainence tool), and not MinGW. WebEngine will only compile with msvc.

## Linux
Install `cmake` and a c++ compiler of your choice.(`clang` or `g++`).

```sh
git clone https://github.com/Milansuman/project-web.git
cd project-web
mkdir build && cd build
cmake .. && make
```

A binary named `project-web` will be created in the build directory.

## Windows
Install Visual Studio Community Edition for the msvc compiler(The build tools are not provided by qt). Ensure `cmake`, `msvc` and `windows 11 sdk` are installed.

Install CMake:
```powershell
winget install cmake
```

`MSVC` and the `Windows 11 SDK` are provided by the Visual Studio installer.

```powershell
git clone https://github.com/Milansuman/project-web.git
cd project-web && mkdir build
cd build
cmake -DQt6_DIR:PATH=<path_to_qt_msvc>/lib/cmake/Qt6 -DQT_ADDITIONAL_PACKAGES_PREFIX_PATH=<path_to_qt_msvc> ..
cmake --build . --config Release
```

The `project-web.exe` binary will be in the build/Release folder.

Ensure the Qt6/bin folder is in $PATH
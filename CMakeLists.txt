cmake_minimum_required(VERSION 3.14)
project(2view LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets Gui REQUIRED)

add_executable(2view main.cpp canvas.cpp)

target_link_libraries(2view PRIVATE Qt6::Widgets Qt6::Gui)

install(TARGETS 2view
    RUNTIME DESTINATION $ENV{HOME}/.local/bin
)

install(CODE "file(WRITE \"\$ENV{HOME}/.local/share/applications/2view.desktop\" [[
[Desktop Entry]
Name=2view
Exec=2view %F
Icon=accessories-image-viewer
MimeType=image/png;image/jpeg;image/bmp;image/webp
Type=Application
Terminal=False
Categories=Graphics;Viewer;Utility;
]])
    message(STATUS \"Installed 2view.desktop\")
")
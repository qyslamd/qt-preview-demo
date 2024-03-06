cmake_minimum_required(VERSION 3.5)

project(qt-preview-demo LANGUAGES CXX VERSION 1.0)

# Set C++ standard version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# *********************find package Qt*****************
# Qt5 UIC
set(CMAKE_AUTOUIC ON)
# Qt5 MOC
set(CMAKE_AUTOMOC ON)
# Qt5 RCC
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets Svg REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Svg REQUIRED)


# *********************make a static libray of chrom tab style*****************
add_library(chromTabStyle STATIC ${CMAKE_CURRENT_SOURCE_DIR}/chrome_tabbar_style/tabbarstyle.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/chrome_tabbar_style/tabbarstyle.h)
target_link_libraries(chromTabStyle PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)


add_subdirectory(qpainter_drawings)
add_subdirectory(click_water_ripples)



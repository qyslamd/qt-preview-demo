# Set C++ standard version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt5 UIC
set(CMAKE_AUTOUIC ON)
# Qt5 MOC
set(CMAKE_AUTOMOC ON)
# Qt5 RCC
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

set(RES res.qrc)
set(ALL_SRC main.cpp
            bagua.cpp
            bagua.h
            bagua_round.cpp
            bagua_round.h
            widget.cpp
            widget.h)
			
add_executable(qpainter-drawings ${ALL_SRC} ${RES})

target_link_libraries(qpainter-drawings PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

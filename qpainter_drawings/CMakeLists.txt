set(RES res.qrc)
set(ALL_SRC main.cpp
            widget.cpp
            widget.h
            bagua.cpp
            bagua.h
            bagua_round.cpp
            bagua_round.h
            oval_seal.cpp
            oval_seal.h)
			
add_executable(qpainter-drawings ${ALL_SRC} ${RES})

target_include_directories(qpainter-drawings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../chrome_tabbar_style)
target_link_libraries(qpainter-drawings PRIVATE Qt${QT_VERSION_MAJOR}::Widgets chromTabStyle)

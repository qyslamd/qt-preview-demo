set(RES res.qrc)
set(ALL_SRC main.cpp
    widget.cpp
    widget.h
    bagua.cpp
    bagua.h
    bagua_round.cpp
    bagua_round.h
    bagua_round_thread.cpp
    bagua_round_thread.h
    oval_seal.cpp
    oval_seal.h
    progress/hourglass.cpp
    progress/hourglass.h
    speedometer.cpp
    speedometer.h
    cplusplus_logo.cpp
    cplusplus_logo.h
    wifi_logo.cpp
    wifi_logo.h
    progress/win10_loading_progress.cpp
    progress/win10_loading_progress.h
    progress/water_progress.cpp
    progress/water_progress.h
    progress/nine_ball_progress.cpp
    progress/nine_ball_progress.h
    progress/four_fish_progress.cpp
    progress/four_fish_progress.h
    progress/ball_stair_progress.cpp
    progress/ball_stair_progress.h)
			
add_executable(qpainter-drawings ${ALL_SRC} ${RES})

target_include_directories(qpainter-drawings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../chrome_tabbar_style)
target_link_libraries(qpainter-drawings PRIVATE
                     Qt${QT_VERSION_MAJOR}::Widgets
                     Qt${QT_VERSION_MAJOR}::Svg
                     chromTabStyle)

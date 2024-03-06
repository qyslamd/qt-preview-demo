set(ALL_SRC main.cpp
    widget.cpp
    widget.h
    click_water_ripple_widget.cpp
    click_water_ripple_widget.h
    click_ripple_listview.cpp
    click_ripple_listview.h)

add_executable(click_water_ripples ${ALL_SRC})

target_link_libraries(click_water_ripples PRIVATE
                     Qt${QT_VERSION_MAJOR}::Widgets)

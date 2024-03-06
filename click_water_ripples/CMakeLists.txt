set(ALL_SRC main.cpp
click_water_ripple_widget.cpp
click_water_ripple_widget.h)

add_executable(click_water_ripples ${ALL_SRC})

target_link_libraries(click_water_ripples PRIVATE
                     Qt${QT_VERSION_MAJOR}::Widgets)

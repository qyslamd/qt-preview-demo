set(ALL_SRC main.cpp
    widget.cpp
    widget.h
    widget.ui
windows_helper.hpp
existed_cfg_dialog.cpp
existed_cfg_dialog.h
existed_cfg_dialog.ui
data_def.h
data_def.cpp
app_settings.cpp
app_settings.h
add_modify_cfg_dialog.cpp
add_modify_cfg_dialog.h
add_modify_cfg_dialog.ui)

set(RES res.qrc)

# 增加 UNICODE 宏定义
add_definitions(-DUNICODE)

add_definitions(-DUSE_CPP17)

add_executable(embed_window_qt ${ALL_SRC} ${RES})

target_link_libraries(embed_window_qt PRIVATE
                     Qt${QT_VERSION_MAJOR}::Widgets
                 Qt${QT_VERSION_MAJOR}::Svg)

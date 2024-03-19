set(TARGET_NAME "qprocess_usage")

set(ALL_SRC main.cpp
            process_manager.cpp
            process_manager.h)

add_executable(${TARGET_NAME} ${ALL_SRC})
target_link_libraries(${TARGET_NAME} PRIVATE
                     Qt${QT_VERSION_MAJOR}::Core)

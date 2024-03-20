set(target_name demo_thread_pool)

set(all_src main.cpp
task_queue.hpp
thread_pool.hpp)

add_executable(${target_name} ${all_src})

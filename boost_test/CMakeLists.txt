set(BOOST_ROOT C:/boost)
set(Boost_LIBRARY_DIRS c:/boost/lib)

find_package(Boost 1.82 REQUIRED COMPONENTS json system filesystem)
message(Boost\ version: ${Boost_VERSION_STRING})
message(Boost\ include\ dir: ${Boost_INCLUDE_DIRS})
message(Boost\ library\ dir: ${Boost_LIBRARY_DIRS})

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})


message("Found Boost Libraries:")
foreach(boost_lib IN LISTS Boost_LIBRARIES)
    message(${boost_lib})
    string(REGEX MATCH ".+/.*boost_([^-]+)-.+\.(lib|a)" boost_lib_name ${boost_lib})
    set(boost_lib_name ${CMAKE_MATCH_1})
    set(boost_target Boost::${boost_lib_name})
    if(TARGET ${boost_target})
        message(STATUS "Boost target found: " ${boost_target})
    endif(TARGET ${boost_target})
endforeach(boost_lib)


set(ALL_SRCS main.cpp
            process_usage.hpp
        json_usage.hpp
    filesystem_usage.hpp
asio_usage.hpp)

set(Target_Name boost_test)
add_executable(${Target_Name} ${ALL_SRCS})

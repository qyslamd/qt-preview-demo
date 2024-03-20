#pragma once

#include <boost/filesystem.hpp>
#include <iostream>

namespace filesystem_usage
{
void myFirstTest()
{
    namespace bf = boost::filesystem;

    bf::path path = bf::current_path();

    std::cout<<path.generic_path()<<std::endl;
}
}   // namespace filesystem_usage

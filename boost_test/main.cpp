#include <iostream>

#include "json_usage.hpp"
#include "process_usage.hpp"

int main(int argc, char* argv[]) {
  std::cout << "This process args:" << std::endl;
  for (int i = 0; i < argc; i++) {
    std::cout << "\t" << argv[i] << std::endl;
  }

  auto ret = process_usage::read_outline(
      R"(D:\Qt512\Tools\mingw730_32\i686-w64-mingw32\lib\libapcups.a)");
  for (auto const& item : ret) {
    std::cout << "\t" << item << " ";
  }
  // json_usage::testBoostJson();
  return 0;
}

#include <iostream>

#include "asio_usage.hpp"
#include "json_usage.hpp"
#include "process_usage.hpp"

int main(int argc, char* argv[]) {
  std::cout << "This process args:" << std::endl;
  for (int i = 0; i < argc; i++) {
    std::cout << "\t" << argv[i] << std::endl;
  }

#if 0
  auto ret = process_usage::testProcessCallPing();
  std::cout << "count of result:" << ret.size() << std::endl;
  for (auto const& item : ret) {
    std::cout << "\t" << item << std::endl;
  }
#endif

#if 0
  json_usage::testBoostJson();
#endif

  return asio_usage::sync_http_client(argc, argv);
  return 0;
}

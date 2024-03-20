#pragma once

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace process_usage {

std::vector<std::string> testProcessCallPing() {
  boost::process::ipstream pipe_stream;
  boost::process::child c("ping www.tencent.com",
                          boost::process::std_out > pipe_stream);

  std::vector<std::string> result;
  std::string line;
  while (pipe_stream && std::getline(pipe_stream, line) && !line.empty()) {
    //    std::cerr << line << std::endl;
    result.push_back(line);
  }

  c.wait();
  std::cout << "child process exit code : " << c.exit_code() << std::endl;
  return result;
}

void first_step() {
  std::string exec = R"(D:\works\git-repos\boost_1_82_0\b2.exe)";
  std::vector<std::string> args;
  args.push_back("b2.exe");
  args.push_back("--version");

  boost::process::child child(exec, args);

  child.wait();
  auto ret = child.exit_code();
  std::cout << "child exit code is: " << ret << std::endl;
}

}  // namespace process_usage

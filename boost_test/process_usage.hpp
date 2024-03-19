#pragma once

#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace process_usage {
namespace bp = boost::process;

void testBoostProcess() {
  boost::process::ipstream pipe_stream;
  boost::process::child child_process("ping www.baidu.com",
                                      boost::process::std_out > pipe_stream);

  std::string line;
  while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
    std::cout << line << std::endl;

  child_process.wait();
  auto ret = child_process.exit_code();
  std::cout << "child process exit code : " << ret << std::endl;
}

void first_step() {
  std::string exec = R"(E:\boost_1_82_0\b2.exe)";
  std::vector<std::string> args;
  args.push_back("b2.exe");
  args.push_back("--version");

  // boost::process::context ctx;
  boost::process::child child(exec, args);

  child.wait();
  auto ret = child.exit_code();
  std::cout << "child exit code is: " << ret << std::endl;
}

std::vector<std::string> read_outline(const std::string& file) {
  bp::ipstream is;  // reading pipe-stream
  bp::child c(R"(D:\Qt512\Tools\mingw730_32\bin\nm.exe)", file,
              bp::std_out > is);

  std::vector<std::string> data;
  std::string line;

  while (c.running() && std::getline(is, line) && !line.empty())
    data.push_back(line);

  c.wait();

  return data;
}
}  // namespace process_usage

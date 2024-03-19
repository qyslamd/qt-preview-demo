#include <boost/process.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <sstream>

void testBoostJson() {
  boost::property_tree::ptree pt;
  pt.put("hello", 2);
  pt.put("world", 3.14);
  pt.put("sb.nima", 3.14);
  pt.put("sb.angela", 3.14);
  pt.put("sb.bob", "[]");

  std::stringstream ss;
  boost::property_tree::write_json(ss, pt);
  std::cout << ss.str() << std::endl;
}

void testBoostProcess() {
  boost::process::ipstream pipe_stream;
  boost::process::child child_process("ping www.baidu.com",
                                      boost::process::std_out > pipe_stream);

  std::string line;
  while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
    std::cout << line << std::endl;

  child_process.wait();
}

int main(int argc, char *argv[]) {
  //  testBoostProcess();

  std::cout << std::flush;

  testBoostJson();
  return 0;
}

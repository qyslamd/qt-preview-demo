#pragma once
#include <boost/json.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <sstream>

namespace json_usage {

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
}  // namespace json_usage

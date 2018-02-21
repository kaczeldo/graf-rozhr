#include "utility.hpp"

const std::string load_file_to_string(const std::string &file_name) {
  std::ifstream infile{file_name};
  return {std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
}
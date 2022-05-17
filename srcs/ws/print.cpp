#include "main.hpp"

void ws::printE(std::string const& msg, std::string const& divider) {
  std::cerr << msg << divider;
}

void ws::print(std::string const& msg, std::string const& divider) {
  std::cout << msg << divider;
}

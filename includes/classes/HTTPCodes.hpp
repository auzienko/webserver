#ifndef HTTPCODES_HPP
#define HTTPCODES_HPP

#define HTTPCodesCOUNT 62

#include <iostream>
#include <sstream>

class HTTPCodes {
 private:
  struct pair {
    int code;
    std::string description;
  };
  static HTTPCodes::pair _types[HTTPCodesCOUNT];
  HTTPCodes(HTTPCodes const& src);
  HTTPCodes& operator=(HTTPCodes const& rhs);

 public:
  HTTPCodes(void);
  ~HTTPCodes(void);
  static std::string getHTTPCodeString(int const& code);
};

#endif

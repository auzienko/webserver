#ifndef CONFIGUTILS_HPP
#define CONFIGUTILS_HPP

#include "../main.hpp"

class ConfigUtils {
 private:
  ConfigUtils(void);
  ConfigUtils(ConfigUtils const& src);
  ConfigUtils& operator=(ConfigUtils const& rhs);

 public:
  ~ConfigUtils(void);

  static std::pair<std::string, t_location const*> getLocationSettings(
      t_server const& s, std::string const& request_path);
};

#endif

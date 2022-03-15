#include "../../includes/classes/ConfigUtils.hpp"

ConfigUtils::~ConfigUtils(void){}

std::pair<std::string, t_location const*> ConfigUtils::getLocationSettings(
    t_server const& s, std::string const& request_path) {
  size_t len = request_path.length();
  if (!len) return std::make_pair("!__empty__path__", nullptr);
  while (--len && request_path.at(len) != '/')
    ;
  if (!len && request_path.at(len) != '/')
    return std::make_pair("!__backslash__not__found__in__path__", nullptr);
  std::string loc(request_path, 0, len + 1);
  std::string fl(request_path, len + 1, request_path.length());
  std::vector<t_location>::const_iterator i = s.locations.begin();
  std::vector<t_location>::const_iterator e = s.locations.end();
  while (i != e) {
    if ((*i).path == loc) {
      if (fl.length())
        return std::make_pair(fl, &(*i));
      else
        return std::make_pair((*i).index, &(*i));
    }
    ++i;
  }
  return std::make_pair(request_path, &(s.locations.at(0)));
}

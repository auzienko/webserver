#ifndef CONFIG_HPP
#define CONFIG_HPP

#define WHITE_SPACES "\n\t "
#include "../main.hpp"

class Config {
 private:
  std::string   _configFile;
  std::ifstream _fileStream;
  int           _debugLine;
  t_server      _server;

  Config(void);
  Config(Config const& src);
  Config& operator=(Config const& rhs);

  void  _setDefaultServer(void);
  void  _locationArgs(std::string &line);
  void  _serverArgs(std::string &line, bool &is_location, bool &is_inlocation);
  void  _locationNameLine(std::string &line, bool &is_location, bool &is_inlocation);
  void  _serverNameLine(std::string &line, bool &is_server, bool &is_inserver);

 public:
  Config(std::string const& file_name);
  ~Config(void);

  void      close(void);
  int       checkAndParse(void);
  t_server  &get_server(void);
};

#endif

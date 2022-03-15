#ifndef WEBSERVERMANAGER_HPP
#define WEBSERVERMANAGER_HPP

#include "../main.hpp"

class Config;
class Webserver;

class WebserverManager {
 private:
  Config* _config;
  std::vector<Webserver*> _webservers;

 public:
  WebserverManager(std::string const& config_path);
  ~WebserverManager();
  int start(void);

 private:
  WebserverManager(void);
  WebserverManager(WebserverManager const& src);
  WebserverManager& operator=(WebserverManager const& rhs);
  int _Create_webserver(t_server &newServerConfig);
  void _Banner(void) const;
};

#endif

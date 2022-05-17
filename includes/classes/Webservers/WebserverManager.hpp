#ifndef WEBSERVERMANAGER_HPP
#define WEBSERVERMANAGER_HPP

#include "classes/Config.hpp"
#include "Webserver.hpp"

class Config;
class Webserver;

class WebserverManager {
 private:
  Config* _config;
  std::vector<Webserver*> _webservers;
  int _maxFd;

 public:
  explicit WebserverManager(std::string const& config_path);
  ~WebserverManager();
  int start(void);

 private:
  WebserverManager(void);
  WebserverManager(WebserverManager const& src);
  WebserverManager& operator=(WebserverManager const& rhs);
  int _Create_webserver(t_server &newServerConfig);
  void _Banner(void) const;
  fd_set _GetAllSocketsFds(void);
  void _ReadHandler(int fd);
  void _OtherHandler(int fd);
};

#endif

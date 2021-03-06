#include "classes/Webservers/WebserverManager.hpp"

WebserverManager::WebserverManager(std::string const &config_path)
    : _maxFd(-1) {
  int res;
  _config = new Config(config_path);
  if (!_config->is_open()) {
    std::cerr << "Config file not found" << std::endl;
    exit(-1);
  }
  res = 1;
  while (res > 0) {
    try {
      res = _config->checkAndParse();
      if (res > 0) _Create_webserver(_config->get_server());
    } catch (...) {
      delete _config;
      throw;
    }
  }
  _config->close();
}

int WebserverManager::start(void) {
  int select_result;

  _Banner();
  std::vector<Webserver *>::iterator it = _webservers.begin();
  std::vector<Webserver *>::iterator e = _webservers.end();
  while (it != e) {
    (*it)->run();
    ++it;
  }
  fd_set all_fds;
  fd_set read_fds;
  fd_set write_fds;

  FD_ZERO(&all_fds);
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);

  while (1) {
    all_fds = _GetAllSocketsFds();
    read_fds = all_fds;
    write_fds = all_fds;
    select_result = select(_maxFd + 1, &read_fds, &write_fds, 0, 0);
    if (select_result < 0) {
      ws::printE(ERROR_SELECT, "\n");
      continue;
    }
    int max_fd = _maxFd;
    for (int i = 0; i <= max_fd && select_result > 0; ++i) {
      if (FD_ISSET(i, &read_fds)) {
        _ReadHandler(i);
        --select_result;
      }
      else if (FD_ISSET(i, &write_fds)) {
        _OtherHandler(i);
        --select_result;
      }
    }
    std::vector<Webserver*>::iterator it = _webservers.begin();
    std::vector<Webserver*>::iterator en = _webservers.end();
    while (it != en) {
      (*it)->closeConnectionIfTimout(TIMEOUT);
      ++it;
    }
    it = _webservers.begin();
    while (it != en) {
      (*it)->closeConnectionIfDone();
      ++it;
    }
  }
  return 0;
}

WebserverManager::~WebserverManager() {
  delete _config;
  std::vector<Webserver *>::iterator i = _webservers.begin();
  std::vector<Webserver *>::iterator e = _webservers.end();
  for (; i != e; ++i) delete (*i);
}

int WebserverManager::_Create_webserver(t_server &newServerConfig) {
  _webservers.push_back(new Webserver(newServerConfig));
  return 0;
}

void WebserverManager::_Banner(void) const {
  ws::print("==================================", "\n");
  ws::print(PROGRAMM_NAME, " ");
  ws::print(PROGRAMM_VERSION, "\n");
  ws::print(AUTHORS, "\n");
  ws::print("==================================", "\n");
}

fd_set WebserverManager::_GetAllSocketsFds(void) {
  fd_set result;
  FD_ZERO(&result);
  std::vector<Webserver *>::iterator i = _webservers.begin();
  std::vector<Webserver *>::iterator e = _webservers.end();
  while (i != e) {
    (*i)->makeActiveFdsSet(&result, &_maxFd);
    ++i;
  }
  return result;
}

void WebserverManager::_ReadHandler(int fd) {
  std::vector<Webserver *>::iterator i = _webservers.begin();
  std::vector<Webserver *>::iterator e = _webservers.end();
  while (i != e) {
    if ((*i)->readHandler(fd) == 0) return;
    ++i;
  }
}

void WebserverManager::_OtherHandler(int fd) {
  std::vector<Webserver *>::iterator i = _webservers.begin();
  std::vector<Webserver *>::iterator e = _webservers.end();
  while (i != e) {
    if ((*i)->otherHandler(fd) == 0) return;
    ++i;
  }
}

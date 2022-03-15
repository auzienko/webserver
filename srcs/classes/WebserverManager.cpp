#include "../../includes/classes/WebserverManager.hpp"

WebserverManager::WebserverManager(std::string const& config_path) {
  int res;
  _config = new Config(config_path);     //Нужна проверка найден файл или нет
  res = 1;
  while (res > 0)
  {
    res = _config->checkAndParse();
    if (res >= 0)
      _Create_webserver(_config->get_server());
  }
  if (res < 0) {
    delete _config;
    //_Delete_webservers();             Если ошибка в конфиге на очередном сервере, прошлые не валидны?
    throw new std::exception;
  }
}

int WebserverManager::start(void) {
  _Banner();
  //пока один
  _webservers[0]->run();
  return 0;
}

WebserverManager::~WebserverManager() {
  delete _config;
  std::vector<Webserver*>::iterator i = _webservers.begin();
  std::vector<Webserver*>::iterator e = _webservers.end();
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

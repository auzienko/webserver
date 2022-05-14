#include "classes/Connections/ConnectionManager.hpp"

ConnectionManager::ConnectionManager(Webserver* webserver)
    : _webserver(webserver) {}

ConnectionManager::~ConnectionManager() {
  std::map<int, AConnection*>::iterator it = _list.begin();
  if (it != _list.end()) {
    delete (*it).second;
    _list.erase(it);
  }
}

void ConnectionManager::add(AConnection* connection) {
  _list.insert(std::pair<int, AConnection*>(connection->getFd(), connection));
}

void ConnectionManager::remove(int fd) {
  std::map<int, AConnection*>::iterator it;
  it = _list.find(fd);
  if (it != _list.end()) {
    delete (*it).second;
    _list.erase(it);
  }
}

AConnection* ConnectionManager::at(int fd) const {
  std::map<int, AConnection*>::const_iterator it;
  it = _list.find(fd);
  if (it != _list.end()) {
    return (*it).second;
  }
  return nullptr;
}

int ConnectionManager::hasDataToReadEvent(int fd) {
  AConnection* tmp = at(fd);
  try {
    if (tmp != nullptr) return (tmp->io() == 1 ? fd : 0);
  } catch (const std::exception &ex) {
    tmp->error(ex);
  }
  return 0;
}

int ConnectionManager::readyToAcceptDataEvent(int fd) {
  AConnection* tmp = at(fd);
  try {
    if (tmp != nullptr) return tmp->io();
  } catch (const std::exception &ex) {
    tmp->error(ex);
  }
  return 0;
}

int ConnectionManager::isExist(int const fd) const {
  if (_list.find(fd) == _list.end()) return 0;
  return 1;
}

std::vector<int> ConnectionManager::getAllConnectionsFds(void) const {
  std::vector<int> result;
  std::map<int, AConnection*>::const_iterator i = _list.begin();
  std::map<int, AConnection*>::const_iterator e = _list.end();
  while (i != e) {
    result.push_back(i->first);
    ++i;
  }
  return result;
}

int ConnectionManager::getConnectionCount(void) const { return _list.size(); }

void ConnectionManager::closeConnectionIfTimout(int seconds) {
  std::map<int, AConnection*>::iterator i = _list.begin();
  std::map<int, AConnection*>::iterator e = _list.end();
  while (i != e) {
    if (std::time(0) - i->second->getLastActivity() > seconds) {
      std::cout << " close timeout" << std::endl;
      delete i->second;
      _list.erase(i++);
    } else
      ++i;
  }
}

Webserver const* ConnectionManager::getWebserver(void) const {
  return _webserver;
}

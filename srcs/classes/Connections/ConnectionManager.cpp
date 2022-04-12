#include "../../../includes/classes/Connections/ConnectionManager.hpp"

ConnectionManager::ConnectionManager() {}

ConnectionManager::~ConnectionManager() {
  std::map<int, AConnection*>::iterator it;
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

int ConnectionManager::readData(int fd) {
  AConnection* tmp = at(fd);
  if (tmp != nullptr) return tmp->readData();
  return 0;
}

int ConnectionManager::handleData(int fd) {
  AConnection* tmp = at(fd);
  if (tmp != nullptr) return tmp->handleData();
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
      delete i->second;
      _list.erase(i++);
    } else
      ++i;
  }
}

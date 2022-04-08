#include "../../includes/classes/ConnectionManager.hpp"

ConnectionManager::ConnectionManager() {}

ConnectionManager::~ConnectionManager() {
  std::map<int, Connection*>::iterator it;
  if (it != _list.end()) {
    delete (*it).second;
    _list.erase(it);
  }
}

void ConnectionManager::add(int fd) {
  Connection* tmp = new Connection(this, fd);
  tmp->setTask(new Request(tmp, fd));
  _list.insert(std::pair<int, Connection*>(fd, tmp));
}

void ConnectionManager::add(int fd, int parentFd) {
  Connection* tmp = new Connection(this, fd);
  //утечка?
  tmp->setTask(new Request(tmp, fd, parentFd));
  _list.insert(std::pair<int, Connection*>(fd, tmp));
}

void ConnectionManager::remove(int fd) {
  std::map<int, Connection*>::iterator it;
  it = _list.find(fd);
  if (it != _list.end()) {
    delete (*it).second;
    _list.erase(it);
  }
}

Connection* ConnectionManager::at(int fd) const {
  std::map<int, Connection*>::const_iterator it;
  it = _list.find(fd);
  if (it != _list.end()) {
    return (*it).second;
  }
  return nullptr;
}

int ConnectionManager::readData(int fd, t_server const& server_config) {
  Connection* tmp = at(fd);
  if (tmp != nullptr) return tmp->readData(server_config);
  return 0;
}

int ConnectionManager::sendData(int fd) {
  Connection* tmp = at(fd);
  if (tmp != nullptr) return tmp->sendData();
  return 0;
}

int ConnectionManager::isExist(int const fd) const {
  if (_list.find(fd) == _list.end()) return 0;
  return 1;
}

std::vector<int> ConnectionManager::getAllConnectionsFds(void) const {
  std::vector<int> result;
  std::map<int, Connection*>::const_iterator i = _list.begin();
  std::map<int, Connection*>::const_iterator e = _list.end();
  while (i != e) {
    result.push_back(i->first);
    ++i;
  }
  return result;
}

int ConnectionManager::getConnectionCount(void) const { return _list.size(); }

void ConnectionManager::closeConnectionIfTimout(int seconds) {
  std::map<int, Connection*>::iterator i = _list.begin();
  std::map<int, Connection*>::iterator e = _list.end();
  while (i != e) {
    if (std::time(0) - i->second->getLastActivity() > seconds) {
      delete i->second;
      _list.erase(i++);
    } else
      ++i;
  }
}

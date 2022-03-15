#include "../../includes/classes/RequestManager.hpp"

RequestManager::RequestManager() {}

RequestManager::~RequestManager() {
  //тут утечка
}

void RequestManager::add(int fd) {
  Request* tmp = new Request(fd);
  _list.insert(std::pair<int, Request*>(fd, tmp));
}

void RequestManager::remove(int fd) {
  std::map<int, Request*>::iterator it;
  it = _list.find(fd);
  if (it != _list.end()) {
    delete (*it).second;
    _list.erase(it);
  }
}

Request* RequestManager::at(int fd) const {
  std::map<int, Request*>::const_iterator it;
  it = _list.find(fd);
  if (it != _list.end()) {
    return (*it).second;
  }
  return nullptr;
}

int RequestManager::getRequest(int fd, t_server const& server_config) {
  Request* tmp = at(fd);
  if (tmp != nullptr) return tmp->getRequest(server_config);
  return 0;
}

int RequestManager::sendResult(int fd) {
  Request* tmp = at(fd);
  if (tmp != nullptr && tmp->getStatus() == READY_TO_SEND) return tmp->sendResult();
  return 0;
}

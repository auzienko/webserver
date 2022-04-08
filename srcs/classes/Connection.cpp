#include "../../includes/classes/Connection.hpp"

Connection::~Connection() {
  killTask();
  close(_fd);
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🖖 CLOSE FD : "
            << _fd << std::endl;
}

Connection::Connection(ConnectionManager* cm, int fd)
    : _connectionManager(cm), _fd(fd), _task(nullptr) {
  setLastActivity();
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🐣 NEW FD : "
            << _fd << std::endl;
}

void Connection::setLastActivity(void) { _lastActivity = std::time(0); }

std::time_t Connection::getLastActivity(void) const { return _lastActivity; }

ConnectionManager* Connection::getConnectionManager(void) const {
  return _connectionManager;
}

void Connection::setTask(Request* task) { _task = task; }

Request* Connection::getTask(void) const { return _task; }

void Connection::killTask(void) {
  if (_task == nullptr) return;
  delete _task;
  _task = nullptr;
}

int Connection::readData(t_server const& server_config) {
  if (_task == nullptr) _task = new Request(this, _fd);
  _task->getRequest(server_config);
  return 0;
}

int Connection::sendData(void) {
  if (_task->getStatus() == READY_TO_SEND) return _task->sendResult();
  return 0;
}

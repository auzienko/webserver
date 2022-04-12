#include "../../../includes/classes/Connections/AConnection.hpp"

AConnection::AConnection()
    : _connectionManager(nullptr),
      _idFd(-1),
      _task(nullptr) {}

AConnection::~AConnection() {
  killTask();
  close(_idFd);
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🖖 CLOSE FD : "
            << _idFd << std::endl;
}

AConnection::AConnection(ConnectionManager* cm, int fd)
    : _connectionManager(cm),
      _idFd(fd),
      _task(nullptr) {
  setLastActivity();
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🐣 NEW FD : "
            << _idFd << std::endl;
}

void AConnection::setLastActivity(void) { _lastActivity = std::time(0); }

std::time_t AConnection::getLastActivity(void) const { return _lastActivity; }

ConnectionManager* AConnection::getConnectionManager(void) const {
  return _connectionManager;
}

void AConnection::setTask(ATask* task) { _task = task; }

ATask* AConnection::getTask(void) const { return _task; }

void AConnection::killTask(void) {
  if (_task == nullptr) return;
  delete _task;
  _task = nullptr;
}

void AConnection::addToOutput(std::string str) { _output << str; }

int AConnection::getFd(void) { return _idFd; }

std::stringstream& AConnection::getInputData(void) { return _input; }

void AConnection::replaceTask(ATask* newTask) {
  killTask();
  setTask(newTask);
}

#include "../../includes/classes/AConnection.hpp"

AConnection::AConnection()
    : _connectionManager(nullptr),
      _subscriptionFd(-1),
      _sendResultFd(-1),
      _task(nullptr) {}

AConnection::~AConnection() {
  killTask();
  close(_subscriptionFd);
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🖖 CLOSE FD : "
            << _subscriptionFd << std::endl;
}

AConnection::AConnection(ConnectionManager* cm, int inputFd)
    : _connectionManager(cm),
      _subscriptionFd(inputFd),
      _sendResultFd(inputFd),
      _task(nullptr) {
  setLastActivity();
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🐣 NEW FD : "
            << _subscriptionFd << std::endl;
}

AConnection::AConnection(ConnectionManager* cm, int inputFd, int outputFd)
    : _connectionManager(cm),
      _subscriptionFd(inputFd),
      _sendResultFd(outputFd),
      _task(nullptr) {
  setLastActivity();
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🐣 NEW FD : "
            << _subscriptionFd << std::endl;
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

int AConnection::getFd(void) { return _subscriptionFd; }

std::stringstream& AConnection::getInputData(void) { return _input; }

void AConnection::replaceTask(ATask* newTask) {
  killTask();
  setTask(newTask);
}

#include "../../includes/classes/AConnection.hpp"

AConnection::AConnection()
    : _connectionManager(nullptr), _inputFd(-1), _outputFd(-1), _task(nullptr) {}

AConnection::~AConnection() {
  killTask();
  close(_inputFd);
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🖖 CLOSE FD : "
            << _inputFd << std::endl;
}

AConnection::AConnection(ConnectionManager* cm, int inputFd)
    : _connectionManager(cm), _inputFd(inputFd), _outputFd(inputFd), _task(nullptr) {
  setLastActivity();
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🐣 NEW FD : "
            << _inputFd << std::endl;
}

AConnection::AConnection(ConnectionManager* cm, int inputFd, int outputFd)
    : _connectionManager(cm), _inputFd(inputFd), _outputFd(outputFd), _task(nullptr) {
  setLastActivity();
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🐣 NEW FD : "
            << _inputFd << std::endl;
}


void AConnection::setLastActivity(void) { _lastActivity = std::time(0); }

std::time_t AConnection::getLastActivity(void) const { return _lastActivity; }

ConnectionManager* AConnection::getConnectionManager(void) const {
  return _connectionManager;
}

void AConnection::setTask(Request* task) { _task = task; }

Request* AConnection::getTask(void) const { return _task; }

void AConnection::killTask(void) {
  if (_task == nullptr) return;
  delete _task;
  _task = nullptr;
}

int AConnection::readData(t_server const& server_config) {
  if (_task == nullptr) _task = new Request(this, _inputFd);
  _task->getRequest(server_config);
  return 0;
}

void AConnection::addToOutput(std::string str){
  _output << str;
}

int AConnection::getFd(void){
  return _inputFd;
}

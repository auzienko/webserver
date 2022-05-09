#include "classes/Connections/NetworkConnection.hpp"

NetworkConnection::NetworkConnection(
    ConnectionManager* cm, int fd,
    const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
  _type = NETWORK_CONNECTION;
  _wrote = 0;
}

NetworkConnection::~NetworkConnection() {}

int NetworkConnection::hasDataToReadEvent(void) {
  if (_task) _io();
  return 0;
}

int NetworkConnection::readyToAcceptDataEvent(void) {
  if (_task) _io();
  return 0;
}

void NetworkConnection::_io() {
  int taskStatus = _task->getStatus();
  switch (taskStatus) {
    case NEW:
      _task->setStatus(READING);
    case READING:
      _reading();
      break;
    case READY_TO_HANDLE:
    case EXECUTION:
      _task->doTask();
      break;
    case READY_TO_SEND:
    case SENDING:
      _writing();
      break;
    case DONE:
      getConnectionManager()->remove(_idFd);
      break;
    default:
      break;
  }
}

int NetworkConnection::_reading(void) {
  setLastActivity();
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  memset(buf, 0, DEFAULT_BUFLEN);
  nbytes = recv(_idFd, &buf, DEFAULT_BUFLEN - 1, 0);
  if (nbytes == -1) return 0;
  if (nbytes < 0) {
    ws::printE("~~ 😞 Server: read failture", "\n");
    return -1;
  } else if (nbytes == 0) {
    std::cout << "fd (NetworkConnection): " << _idFd << " reading no data\n";
    _task->doTask();
    _input.str(std::string());
    return 0;
  } else {
    _input << buf;
    std::cout << "\n⬇ ⬇ ⬇ fd (NetworkConnection): " << _idFd << " READ "
              << nbytes / 1024. << "Kb data\n";
    _task->doTask();
    _input.str(std::string());
  }
  return 0;
}

int NetworkConnection::_writing(void) {
  setLastActivity();
  int ret = 0;
  int nbytes = 0;
  if (static_cast<std::string::size_type>(_wrote) < _output.str().length()) {
    char buf[DEFAULT_BUFLEN];
    _output.get(buf, DEFAULT_BUFLEN, 0);
    nbytes = send(_idFd, buf, strlen(buf), 0);
    if (nbytes)
      std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection): " << _idFd << " WROTE "
                << nbytes /* / 1024. */ << "B data result code: " << ret
                << std::endl;
    _wrote += nbytes;
    return 0;
  }
  _task->setStatus(DONE);
  if (_output.str().length() < 100) std::cout << _output.str() << std::endl;
  std::cout << _wrote << " bytes wrote totaly of " << _output.str().length()
            << std::endl;
  return ret;
}

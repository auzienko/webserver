#include "classes/Connections/NetworkConnection.hpp"

NetworkConnection::NetworkConnection(
    ConnectionManager* cm, int fd,
    const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
  _type = NETWORK_CONNECTION;
  _wrote = 0;
  _len = 0;
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
  int nbytes = 0;
  if (!_len) _len = _output.str().length();
  if (static_cast<std::string::size_type>(_wrote) < _len) {
    char buf[DEFAULT_BUFLEN];
    _output.read(buf, ((_len - _wrote) > DEFAULT_BUFLEN ? DEFAULT_BUFLEN : _len - _wrote));
    nbytes = send(_idFd, buf, strlen(buf), 0);
    if (nbytes)
      std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection): " << _idFd << " WROTE "
                << nbytes /* / 1024. */ << "B data result code: " << _len
                << std::endl;
    _wrote += nbytes;
    return 0;
  }
  _task->setStatus(DONE);
  if (_len < 100) std::cout << _output.str() << std::endl;
  std::cout << _wrote << " bytes wrote totaly of " << _len
            << std::endl;
  return 0;
}

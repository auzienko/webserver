#include "classes/Connections/NetworkConnection.hpp"

NetworkConnection::NetworkConnection(
    ConnectionManager* cm, int fd,
    const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
  _type = NETWORK_CONNECTION;
}

NetworkConnection::~NetworkConnection() {}

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
              << nbytes << "B data\n";
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
    int size = (_len - _wrote) > DEFAULT_BUFLEN ? DEFAULT_BUFLEN : _len - _wrote;
    _output.rdbuf()->sgetn(_buf, size);
    nbytes = send(_idFd, _buf, size, 0);
    _wrote += nbytes;
    if (nbytes == -1) {
      std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection): " << _idFd << " Error";
    }
    // if (nbytes)
    //   std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection): " << _idFd << " WROTE "
    //             << nbytes << "B data. Progress: " << _wrote
    //             << "/" << _len << std::endl;
    if (static_cast<std::string::size_type>(_wrote) == _len)
      getConnectionManager()->remove(_idFd);
    return 0;
  }
  _task->setStatus(DONE);
  // std::cout << _wrote << " bytes wrote totaly of " << _len << std::endl;
  return 0;
}

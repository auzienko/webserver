#include "classes/Connections/NetworkConnection.hpp"

NetworkConnection::NetworkConnection(
    ConnectionManager* cm, int fd,
    const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {}

NetworkConnection::~NetworkConnection() {}

int NetworkConnection::_reading(void) {
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  memset(buf, 0, DEFAULT_BUFLEN);
  nbytes = recv(_idFd, &buf, DEFAULT_BUFLEN - 1, 0);
  if (nbytes == 0) {
#ifdef DEBUG
    std::cout << "fd (NetworkConnection) #" << _idFd << ": reading no data\n";
#endif

    return -1;
  }
  _task->setLastActivity();
  _input << buf;
  _task->doTask();
  _input.str(std::string());
  return 0;
}

int NetworkConnection::_writing(void) {
  _task->setLastActivity();
  if (!_len) _len = _output.str().length();
  if (static_cast<std::string::size_type>(_wrote) < _len) {
    int size =
        (_len - _wrote) > DEFAULT_BUFLEN ? DEFAULT_BUFLEN : _len - _wrote;
    _output.rdbuf()->sgetn(_buf, size);
    int nbytes = 0;
    nbytes = send(_idFd, _buf, size, 0);
    _wrote += nbytes;

#ifdef DEBUG
    if (nbytes == -1) {
      std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection) #" << _idFd
                << ": Error on sending response";
    }
    if (nbytes)
      std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection): " << _idFd << " WROTE "
                << nbytes << "B data. Progress: " << _wrote << "/" << _len
                << std::endl;
#endif

    return 0;
  }
  _task->setStatus(DONE);

#ifdef DEBUG
  std::cout << _wrote << " bytes wrote totaly of " << _len << std::endl;
#endif

  return 0;
}

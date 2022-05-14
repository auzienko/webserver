#include "classes/Connections/LocalConnection.hpp"

LocalConnection::LocalConnection(ConnectionManager* cm, int fd,
                                 const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
  _type = LOCAL_CONNECTION;
}

LocalConnection::~LocalConnection() {}

int LocalConnection::_reading(void) {
  _task->setLastActivity();
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  memset(buf, 0, DEFAULT_BUFLEN);
  nbytes = read(_idFd, &buf, DEFAULT_BUFLEN - 1);
  if (nbytes == -1) return 0;
  if (nbytes > 0) {
    _input << buf;
    std::cout << "Reading " << strlen(buf) << " from CGI" << std::endl;
    return 0;
  }
  if (nbytes == 0) {
    std::cout << "Empty" << std::endl;
    _task->doTask();
  }
  // if (nbytes == -1) return 0;
  // if (nbytes < 0) {
  //   ws::printE("~~ 😞 Server: read failture", "\n");
  //   return -1;
  // } else if (nbytes == 0) {
  //   std::cout << "fd (LocalConnection) #" << _idFd << ": reading no data\n";
  //   _task->doTask();
  //   return 0;
  // } else {
  //   _input << buf;
  //   std::cout << "\n⬇ ⬇ ⬇ fd (LocalConnection) #" << _idFd << ": READ "
  //             << nbytes << "B data\n";
  // }
  return 0;
}

int LocalConnection::_writing(void) {
  _task->setLastActivity();
  int nbytes = 0;
  if (!_len) _len = _output.str().length();
  if (static_cast<std::string::size_type>(_wrote) < _len) {
    int size = (_len - _wrote) > DEFAULT_BUFLEN ? DEFAULT_BUFLEN : _len - _wrote;
    _output.rdbuf()->sgetn(_buf, size);
    nbytes = write(_idFd, _buf, size);
    std::cout << "Write data to CGI" << std::endl;
    _wrote += nbytes;
    // if (nbytes)
    //   std::cout << "\n⬆ ⬆ ⬆ fd (LocalConnection) #" << _idFd << ": WROTE "
    //             << nbytes << "B data. Progress: " << _wrote
    //             << "/" << _len << std::endl;
    return 0;
  }
  _task->setStatus(DONE);
  std::cout << _wrote << " bytes sended totaly of " << _len << std::endl;
  // getConnectionManager()->remove(_idFd);
  return 0;
}

#include "classes/Connections/NetworkConnection.hpp"

NetworkConnection::NetworkConnection(
    ConnectionManager* cm, int fd,
    const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
  _type = NETWORK_CONNECTION;
}

NetworkConnection::~NetworkConnection() {}

int NetworkConnection::_reading(void) {
  _task->setLastActivity();
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  memset(buf, 0, DEFAULT_BUFLEN);
  nbytes = recv(_idFd, &buf, DEFAULT_BUFLEN - 1, 0);
  // if (nbytes == -1) {
  //    std::cerr << "fd (NetworkConnection) #" << _idFd << ": recv failture\n";
  //   getConnectionManager()->remove(_idFd);
  //   return 0;
  // }
  if (nbytes == 0 || (nbytes == 1 && buf[0] == 4)){   // Close on error
    std::cout << "fd (NetworkConnection) #" << _idFd << ": reading no data\n";
    std::cout << " network close ERROR" << std::endl;
    getConnectionManager()->remove(_idFd);
    return -1;
  }
  _input << buf;
  _task->doTask();
  _input.str(std::string());

  // } else if (nbytes == 0) {
  //   std::cout << "fd (NetworkConnection) #" << _idFd << ": reading no data\n";
  //    _task->doTask();
  //   _input.str(std::string());
  //   getConnectionManager()->remove(_idFd);
  //   return 0;
  // } else {
  //   _input << buf;
  //   // std::cout << "\n⬇ ⬇ ⬇ fd (NetworkConnection) #" << _idFd << ": READ "
  //   //           << nbytes << "B data\n";
  //   //_task->doTask();
  //   //_input.str(std::string());
  //}
  return 0;
}

int NetworkConnection::_writing(void) {
  _task->setLastActivity();
  int nbytes = 0;
  if (!_len) _len = _output.str().length();
  if (static_cast<std::string::size_type>(_wrote) < _len) {
    int size = (_len - _wrote) > DEFAULT_BUFLEN ? DEFAULT_BUFLEN : _len - _wrote;
    _output.rdbuf()->sgetn(_buf, size);
    nbytes = send(_idFd, _buf, size, 0);
    _wrote += nbytes;
    if (nbytes == -1) {
      std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection) #" << _idFd << ": Error";
    }
    // if (nbytes)
    //   std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection): " << _idFd << " WROTE "
    //             << nbytes << "B data. Progress: " << _wrote
    //             << "/" << _len << std::endl;

    // if (static_cast<std::string::size_type>(_wrote) == _len)
    // {
    //   std::cout << " network close WRITE" << std::endl;
    //   getConnectionManager()->remove(_idFd);
    // }
    return 0;
  }
  _task->setStatus(DONE);
  //getConnectionManager()->remove(_idFd);
  // std::cout << _wrote << " bytes wrote totaly of " << _len << std::endl;
  return 0;
}

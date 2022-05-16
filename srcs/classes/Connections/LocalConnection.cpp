#include "../../../includes/classes/Connections/LocalConnection.hpp"
#include "../../../includes/classes/Tasks/CgiOutputTask.hpp"
#include "../../../includes/classes/Tasks/ATask.hpp"

class ATask;
class CgiOutputTask;

LocalConnection::LocalConnection(ConnectionManager* cm, int fd,
                                 const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
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
    dynamic_cast<CgiOutputTask *>(_task)->addLength(strlen(buf));

#ifdef DEBUG
    std::cout << "Reading " << strlen(buf) << " from CGI" << std::endl;
#endif

    return 0;
  }
  if (nbytes == 0) {
    _task->doTask();
  }
  return 0;
}

int LocalConnection::_writing(void) {
  _task->setLastActivity();
  if (!_len) _len = _output.str().length();
  if (static_cast<std::string::size_type>(_wrote) < _len) {
    int size = (_len - _wrote) > DEFAULT_BUFLEN ? DEFAULT_BUFLEN : _len - _wrote;
    _output.rdbuf()->sgetn(_buf, size);
    int nbytes = 0;
    nbytes = write(_idFd, _buf, size);
    _wrote += nbytes;

#ifdef DEBUG
    if (nbytes)
      std::cout << "\n⬆ ⬆ ⬆ fd (LocalConnection) #" << _idFd << ": WROTE "
                << nbytes << "B data. Progress: " << _wrote
                << "/" << _len << std::endl;
#endif

    return 0;
  }
  _task->setStatus(DONE);

#ifdef DEBUG
  std::cout << _wrote << " bytes sended to CGI totaly of " << _len << std::endl;
#endif

  return 0;
}

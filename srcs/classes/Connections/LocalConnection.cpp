#include "../../../includes/classes/Connections/LocalConnection.hpp"

LocalConnection::LocalConnection(ConnectionManager* cm, int fd,
                                 const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
  _type = LOCAL_CONNECTION;
}

LocalConnection::~LocalConnection() {}

int LocalConnection::hasDataToReadEvent(void) {
  if (!_task) return 0;
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  memset(buf, 0, DEFAULT_BUFLEN);
  nbytes = read(_idFd, &buf, DEFAULT_BUFLEN - 1);
  if (nbytes == -1) return 0;
  if (nbytes < 0) {
    ws::printE("~~ 😞 Server: read failture", "\n");
    return -1;
  } else if (nbytes == 0) {
    std::cout << "fd (LocalConnection): " << _idFd << " reading no data\n";
    _task->doTask();
    getConnectionManager()->remove(_idFd);
    return 0;
  } else {
    _input << buf;
    std::cout << "\n⬇ ⬇ ⬇ fd (LocalConnection): " << _idFd << " READ " << nbytes / 1024. << "Kb data\n";
  }
  return 0;
}

int LocalConnection::readyToAcceptDataEvent(void) {
  if (!_task) return 0;
  int nbytes, ret = 0;
  if (_task->getStatus() == SENDING) {
    nbytes = write(_idFd, _output.str().c_str(), _output.str().length());
    if (nbytes < 0) ret = -1;
    std::cout << "\n⬆ ⬆ ⬆ fd (LocalConnection): " << _idFd << " WROTE " << nbytes / 1024. << "Kb data result code: " << ret << std::endl;
    _task->setStatus(DONE);
    setLastActivity();

    //когда закрывать коннекшены??? ориентироваться на статусы и кипэлайф
    //наверно.
    // if (!_task->getIsKeepAlive())
    getConnectionManager()->remove(_idFd);
    //остался еще фд. не забудь про аутпут

  } else if (_task->getStatus() >= READY_TO_HANDLE) {
    _task->doTask();
  }
  return ret;
}

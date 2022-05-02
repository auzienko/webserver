#include "../../../includes/classes/Connections/LocalConnection.hpp"

LocalConnection::LocalConnection(ConnectionManager* cm, int fd, const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
  _type = LOCAL_CONNECTION;
}

LocalConnection::~LocalConnection(){}

int LocalConnection::hasDataToReadEvent(void) {
  if (!_task) return 0;
  //тут подумать еще.
  if (_task->getStatus() >= READY_TO_HANDLE) return 0;
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  memset(buf,0,DEFAULT_BUFLEN);
  nbytes = read(_idFd, &buf, DEFAULT_BUFLEN);
  if (nbytes == -1) return 0;
  if (nbytes < 0) {
    ws::printE("~~ 😞 Server: read failture", "\n");
    return -1;
  } else if (nbytes == 0) {
    std::cout << "fd (LocalConnection): " << _idFd << " reading no data\n";
    return 0;
  } else {
    _input << buf;
  }
  _task->doTask();
  return 0;
}

int LocalConnection::readyToAcceptDataEvent(void) {
  if (!(_task && _task->getStatus() == SENDING)) return 0;

  int nbytes, ret = 0;

  nbytes = write(_idFd, _output.str().c_str(), _output.str().length());
  if (nbytes < 0) ret = -1;
  printf("Server: write return %d, connection %d ", ret, _idFd);
  _task->setStatus(DONE);
  setLastActivity();
  //когда закрывать коннекшены??? ориентироваться на статусы и кипэлайф наверно.
  getConnectionManager()->remove(_idFd);
  //не забудь закрыть оутпут

  return ret;
}

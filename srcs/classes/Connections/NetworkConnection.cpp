#include "../../../includes/classes/Connections/NetworkConnection.hpp"

NetworkConnection::NetworkConnection(
    ConnectionManager* cm, int fd,
    const std::map<int, std::string>* error_pages)
    : AConnection(cm, fd, error_pages) {
  _type = NETWORK_CONNECTION;
}

NetworkConnection::~NetworkConnection() {}

int NetworkConnection::hasDataToReadEvent(void) {
  if (!_task) return 0;
  //тут подумать еще.
  // if (_task->getStatus() >= READY_TO_HANDLE) return 0;
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

    getConnectionManager()->remove(_idFd);  // См. 48

    return 0;
  } else {
    _input << buf;
    std::cout << "\n⬇ ⬇ ⬇ fd (NetworkConnection): " << _idFd << " READ " << nbytes / 1024. << "Kb data\n";
    std::cout << _input.str() << std::endl;
  }
  _task->doTask();
  return 0;
}

int NetworkConnection::readyToAcceptDataEvent(void) {
  if (!_task) return 0;
  int ret = 0;
  if (_task->getStatus() == SENDING) {
    int nbytes = send(_idFd, _output.str().c_str(), _output.str().length(), 0);
    if (nbytes < 0) ret = -1;
    std::cout << "\n⬆ ⬆ ⬆ fd (NetworkConnection): " << _idFd << " WROTE " << nbytes / 1024. << "Kb data result code: " << ret << std::endl;
    _task->setStatus(DONE);
    setLastActivity();

    //когда закрывать коннекшены??? ориентироваться на статусы и кипэлайф
    //наверно.
    if (!_task->getIsKeepAlive()) getConnectionManager()->remove(_idFd);
    //остался еще фд. не забудь про аутпут

  } else if (_task->getStatus() >= READY_TO_HANDLE) {
    _task->doTask();
  }
  return ret;
}

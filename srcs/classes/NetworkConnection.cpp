#include "../../includes/classes/NetworkConnection.hpp"

NetworkConnection::NetworkConnection(ConnectionManager* cm, int inputFd)
    : AConnection(cm, inputFd, inputFd) {
  _type = NETWORK_CONNECTION;
}

NetworkConnection::NetworkConnection(ConnectionManager* cm, int inputFd,
                                     int outputFd)
    : AConnection(cm, inputFd, outputFd) {
  _type = NETWORK_CONNECTION;
}

NetworkConnection::~NetworkConnection() {}

int NetworkConnection::readData(void) {
  if (!_task) return 0;
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  int fd = _task->getFd();

  nbytes = recv(fd, &buf, DEFAULT_BUFLEN, 0);
  if (nbytes == -1) return 0;
  if (nbytes < 0) {
    ws::printE("~~ 😞 Server: read failture", "\n");
    return -1;
  } else if (nbytes == 0) {
    std::cout << "fd: " << fd << " reading no data\n";
    return 0;
  } else {
    _input << buf;
  }
  _task->doTask();
  return 0;
}

int NetworkConnection::handleData(void) {
  if (!_task) return 0;
  int ret = 0;
  if (_task->getStatus() == SENDING) {
    int nbytes =
        send(_sendResultFd, _output.str().c_str(), _output.str().length(), 0);
    if (nbytes < 0) ret = -1;
    printf("Server: write return %d, connection %d ", ret, _sendResultFd);
    _task->setStatus(DONE);
    setLastActivity();

    //когда закрывать коннекшены??? ориентироваться на статусы и кипэлайф
    //наверно.
    getConnectionManager()->remove(_subscriptionFd);
    //остался еще фд. не забудь про аутпут

  } else if (_task->getStatus() >= READY_TO_HANDLE) {
    _task->doTask();
  }
  return ret;
}

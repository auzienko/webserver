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

int NetworkConnection::sendData(void) {
  if (!(_task != nullptr && _task->getStatus() >= SENDING &&
        _task->getStatus() != DONE))
    return 0;
  int nbytes, ret;
  ret = 0;
  nbytes = send(_outputFd, _output.str().c_str(), _output.str().length(), 0);
  if (nbytes < 0) ret = -1;
  printf("Server: write return %d, connection %d ", ret, _outputFd);
  _task->setStatus(DONE);
  setLastActivity();
  //когда закрывать коннекшены??? ориентироваться на статусы и кипэлайф наверно.
  getConnectionManager()->remove(_inputFd);
  //остался еще фд. не забудь про аутпут

  return ret;
}

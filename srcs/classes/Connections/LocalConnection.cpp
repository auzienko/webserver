#include "../../../includes/classes/Connections/LocalConnection.hpp"

LocalConnection::LocalConnection(ConnectionManager* cm, int inputFd)
    : AConnection(cm, inputFd, inputFd) {
  _type = LOCAL_CONNECTION;
}

LocalConnection::LocalConnection(ConnectionManager* cm, int inputFd,
                                     int outputFd)
    : AConnection(cm, inputFd, outputFd) {
  _type = LOCAL_CONNECTION;
}


LocalConnection::~LocalConnection(){}

int LocalConnection::readData(void) {
  return 0;
}

int LocalConnection::handleData(void) {
  if (!(_task && _task->getStatus() == SENDING)) return 0;

  int nbytes, ret = 0;

  nbytes = write(_sendResultFd, _output.str().c_str(), _output.str().length());
  if (nbytes < 0) ret = -1;
  printf("Server: write return %d, connection %d ", ret, _sendResultFd);
  _task->setStatus(DONE);
  setLastActivity();
  //когда закрывать коннекшены??? ориентироваться на статусы и кипэлайф наверно.
  getConnectionManager()->remove(_subscriptionFd);
  //не забудь закрыть оутпут

  return ret;
}

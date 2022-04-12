#include "../../../includes/classes/Connections/LocalConnection.hpp"

LocalConnection::LocalConnection(ConnectionManager* cm, int fd)
    : AConnection(cm, fd) {
  _type = LOCAL_CONNECTION;
}

LocalConnection::~LocalConnection(){}

int LocalConnection::hasDataToReadEvent(void) {
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

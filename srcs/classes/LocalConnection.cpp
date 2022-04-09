#include "../../includes/classes/LocalConnection.hpp"

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

int LocalConnection::sendData(void) {
  if (!(_task && _task->getStatus() >= READY_TO_SEND &&
        _task->getStatus() != DONE))
    return 0;
  int nbytes, ret;
  ret = 0;

  nbytes = write(_outputFd, _output.str().c_str(), _output.str().length());
  if (nbytes < 0) ret = -1;
  printf("Server: write return %d, connection %d ", ret, _outputFd);
  _task->setStatus(DONE);
  setLastActivity();
  //когда закрывать коннекшены??? ориентироваться на статусы и кипэлайф наверно.
  getConnectionManager()->remove(_inputFd);
  //не забудь закрыть оутпут

  return ret;
}

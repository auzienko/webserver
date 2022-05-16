#include "classes/Tasks/RedirTask.hpp"
#include "classes/MimeTypes.hpp"
#include "classes/HTTPCodes.hpp"

RedirTask::RedirTask(AConnection* connection, int const& fd, t_uriInfo parsedURI)
    : ATask(NETWORK_REDIRECT, fd, connection), _parsedURI(parsedURI) {}

RedirTask::~RedirTask() {}

int RedirTask::collectData(void) { return 0; }

int RedirTask::executeTask(void) {
  if (_MakeHeader(_parsedURI.loc->redir.code) == 0) _AssembleResponse();
  return 0;
}

int RedirTask::_MakeHeader(int status) {
  _response.clear();
  _response << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(status) << "\r\n";
  _response << "Location: " << _parsedURI.loc->redir.target << "\r\n";
  _response << "Connection: Close\r\n";
  return 0;
}

int RedirTask::_AssembleResponse(void) {
  setStatus(READY_TO_SEND);
  sendResult();
  return 0;
}

int RedirTask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  _connection->addToOutput(_response.str());
  setStatus(SENDING);
  return 0;
}

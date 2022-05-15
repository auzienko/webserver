#include "classes/Tasks/DeleteTask.hpp"
#include "classes/HTTPCodes.hpp"

DeleteTask::DeleteTask(AConnection* connection, int const& fd, t_uriInfo parsedURI)
    : ATask(NETWORK_DELETE, fd, connection), _parsedURI(parsedURI) {}

DeleteTask::~DeleteTask() {}

int DeleteTask::collectData(void) { return 0; }

int DeleteTask::executeTask(void) {
  if (_MakeHeader() == 0) _AssembleResponse();
  return 0;
}

int DeleteTask::_MakeHeader() {
  _response.clear();

  remove(_parsedURI.uri.c_str());
  _response << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(204) << "\r\n";
  _response << "Connection: Close\r\n";
  _response << "\r\n";
  return 0;
}

int DeleteTask::_AssembleResponse(void) {
  setStatus(READY_TO_SEND);
  sendResult();
  return 0;
}

int DeleteTask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  _connection->addToOutput(_response.str());
  setStatus(SENDING);
  return 0;
}

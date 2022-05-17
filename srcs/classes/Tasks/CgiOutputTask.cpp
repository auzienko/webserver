#include "classes/Tasks/CgiOutputTask.hpp"
#include "classes/MimeTypes.hpp"
#include "classes/HTTPCodes.hpp"

CgiOutputTask::CgiOutputTask(AConnection* connection, int const& fd,
                             int const& parentFd)
    : ATask(LOCAL_CGI_OUTPUT, fd, connection),
      _parentFd(parentFd), _length(0) {}

CgiOutputTask::~CgiOutputTask() {}

void CgiOutputTask::addLength(int addedLength) {
  _length += addedLength;
}

int CgiOutputTask::collectData(void) {
  setStatus(READY_TO_HANDLE);
  executeTask();
  return 0;
}

int CgiOutputTask::setLastActivity(void) {
  _connection->setLastActivity();
  _connection->getConnectionManager()->at(_parentFd)->getTask()->setLastActivity();
  return 0;
}

int CgiOutputTask::executeTask(void) {
  setStatus(EXECUTION);
  std::cout << "\n~~~ CgiOutputTask::executeTask\n";
  int status = 200;
  std::stringstream result;
  result << "HTTP/1.1 " + HTTPCodes::getHTTPCodeString(status) + "\r\n";
  result << "Connection: Close\r\n";
  std::string tmpInput(_connection->getInputData().str());
  int i = tmpInput.find("\r\n\r\n");
  result << "Content-length: " << _length - (i + 4) << "\r\n\r\n";
  result << &tmpInput.data()[i + 4];

  _connection->getConnectionManager()->at(_parentFd)->addToOutput(result.str());
  _connection->getConnectionManager()->at(_parentFd)->getTask()->setStatus(
      READY_TO_SEND);
  setStatus(DONE);
  return 0;
}

int CgiOutputTask::sendResult(void) {
  std::cout << "\n~~~ CgiOutputTask::sendResult\n";
  return 0;
}

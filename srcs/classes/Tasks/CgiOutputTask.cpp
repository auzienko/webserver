#include "classes/Tasks/CgiOutputTask.hpp"
#include "classes/MimeTypes.hpp"
#include "classes/HTTPCodes.hpp"

CgiOutputTask::CgiOutputTask(AConnection* connection, int const& fd,
                             int const& parentFd)
    : ATask(LOCAL_CGI_OUTPUT, fd),
      _connection(connection),
      _parentFd(parentFd) {}

CgiOutputTask::~CgiOutputTask() {}

int CgiOutputTask::collectData(void) {
  setStatus(READY_TO_HANDLE);
  executeTask();
  return 0;
}

int CgiOutputTask::executeTask(void) {
  setStatus(EXECUTION);
  std::cout << "\n~~~ CgiOutputTask::executeTask\n";
  //status hardcode
  int status = 200;
  std::stringstream result;
  result << "HTTP/1.1 " + HTTPCodes::getHTTPCodeString(status) + "\r\n";
  int i = _connection->getInputData().str().find("\r\n\r\n");
  result << "Content-length: " << _connection->getInputData().str().substr(i + 4).length() << "\r\n\r\n";
  result << _connection->getInputData().str().substr(i + 4);

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

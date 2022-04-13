#include "../../../includes/classes/Tasks/CgiOutputTask.hpp"

#include "../../../includes/classes/MimeTypes.hpp"

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
  std::cout << "~~~ CgiOutputTask::executeTask\n";

  std::stringstream result;
  result << "HTTP/1.1 200 OK\r\n";
  result << "Connection: keep-alive\r\n";
  result << _connection->getInputData().str();

  _connection->getConnectionManager()->at(_parentFd)->addToOutput(result.str());
  _connection->getConnectionManager()->at(_parentFd)->getTask()->setStatus(
      READY_TO_SEND);
  setStatus(DONE);
  return 0;
}

int CgiOutputTask::sendResult(void) {
  std::cout << "~~~ CgiOutputTask::sendResult\n";
  return 0;
}

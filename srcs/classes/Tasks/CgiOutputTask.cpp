#include "../../../includes/classes/Tasks/CgiOutputTask.hpp"
#include "../../../includes/classes/MimeTypes.hpp"

CgiOutputTask::CgiOutputTask(AConnection* connection, int const& fd,
                           int const& parentFd)
    : ATask(LOCAL_CGI_OUTPUT, fd),
      _connection(connection),
      _parentFd(parentFd) {}

CgiOutputTask::~CgiOutputTask() {}

int CgiOutputTask::collectData(void) {
  std::cout << "~~~ CgiOutputTask::collectData:\n" << _connection->getInputData().str() << std::endl;
  setStatus(DONE);
  return 0;
}

int CgiOutputTask::executeTask(void) {
    std::cout << "~~~ CgiOutputTask::executeTask\n";
     return 0; }

int CgiOutputTask::sendResult(void) {
    std::cout << "~~~ CgiOutputTask::sendResult\n";
  return 0;
}

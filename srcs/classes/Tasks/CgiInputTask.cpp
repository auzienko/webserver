#include "../../../includes/classes/Tasks/CgiInputTask.hpp"
#include "../../../includes/classes/MimeTypes.hpp"

CgiInputTask::CgiInputTask(AConnection* connection, int const& fd,
                           int const& parentFd)
    : ATask(LOCAL_CGI_INPUT, fd),
      _connection(connection),
      _parentFd(parentFd) {}

CgiInputTask::~CgiInputTask() {}

int CgiInputTask::collectData(void) { 
std::cout << "\n~~~collectData data to CGI script\n";
    return 0; }

int CgiInputTask::executeTask(void) { 
std::cout << "\n~~~ executeTask data to CGI script\n";
    return 0; }

int CgiInputTask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  setStatus(SENDING);
  std::cout << "\n~~~ Send data to CGI script\n";
  return 0;
}

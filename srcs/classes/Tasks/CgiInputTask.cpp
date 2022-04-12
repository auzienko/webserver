#include "../../../includes/classes/Tasks/CgiInputTask.hpp"
#include "../../../includes/classes/MimeTypes.hpp"

CgiInputTask::CgiInputTask(AConnection* connection, int const& fd,
                           int const& parentFd)
    : ATask(LOCAL_CGI_INPUT, fd),
      _connection(connection),
      _parentFd(parentFd) {}

CgiInputTask::~CgiInputTask() {}

int CgiInputTask::collectData(void) { 
std::cout << "~~~collectData data to CGI script\n";
    return 0; }

int CgiInputTask::executeTask(void) { 
std::cout << "~~~ executeTask data to CGI script\n";
    return 0; }

int CgiInputTask::sendResult(void) {
  setStatus(SENDING);
  std::cout << "~~~ Send data to CGI script\n";
  return 0;
}

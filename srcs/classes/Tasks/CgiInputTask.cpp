#include "classes/Tasks/CgiInputTask.hpp"
#include "classes/MimeTypes.hpp"

CgiInputTask::CgiInputTask(AConnection* connection, int const& fd,
                           int const& parentFd)
    : ATask(LOCAL_CGI_INPUT, fd, connection),
      _parentFd(parentFd) {}

CgiInputTask::~CgiInputTask() {}

int CgiInputTask::setLastActivity(void) {
  _connection->setLastActivity();
  _connection->getConnectionManager()->at(_parentFd)->getTask()->setLastActivity();
  return 0;
}

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

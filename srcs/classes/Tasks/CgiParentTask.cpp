#include "classes/Tasks/CgiParentTask.hpp"

#include "classes/MimeTypes.hpp"

CgiParentTask::CgiParentTask(AConnection* connection, int const& fd,
                             int const& childInputFd, int const& childOutputFd)
    : ATask(NETWORK_CGI_PARENT, fd),
      _connection(connection),
      _childInputFd(childInputFd),
      _childOutputFd(childOutputFd) {}

CgiParentTask::~CgiParentTask() {}

int CgiParentTask::collectData(void) { return 0; }

int CgiParentTask::executeTask(void) { return 0; }

int CgiParentTask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  setStatus(SENDING);
  return 0;
}

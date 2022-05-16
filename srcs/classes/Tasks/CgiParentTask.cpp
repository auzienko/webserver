#include "../../../includes/classes/Tasks/CgiParentTask.hpp"
#include <signal.h>
#include "../../../includes/classes/MimeTypes.hpp"

CgiParentTask::CgiParentTask(AConnection* connection, int const& fd,
                             int const& childInputFd, int const& childOutputFd, int pid)
    : ATask(NETWORK_CGI_PARENT, fd, connection),
      _childInputFd(childInputFd),
      _childOutputFd(childOutputFd), _pid(pid) {}

CgiParentTask::~CgiParentTask() {
  if (kill(_pid, SIGINT) == 0) waitpid(_pid, NULL, 0);
}

int CgiParentTask::collectData(void) { return 0; }

int CgiParentTask::executeTask(void) { return 0; }

int CgiParentTask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  setStatus(SENDING);
  return 0;
}

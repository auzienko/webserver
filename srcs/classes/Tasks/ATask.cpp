#include "../../../includes/classes/Tasks/ATask.hpp"

ATask::ATask(int type, int fd) : _fd(fd), _status(NEW) {
  switch (type) {
    case UNKNOWN_NETWORK:
    case NETWORK_AUTOINDEX:
    case NETWORK_GET:
    case NETWORK:
    case CGI:
      _type = type;
      break;
    default:
      _type = UNKNOWN_NETWORK;
      break;
  }
}

ATask::~ATask() {}

int ATask::getStatus(void) const { return _status; }

int ATask::getType(void) const { return _type; }
int ATask::getFd(void) const { return _fd; }

void ATask::setStatus(int status) {
  if (status > NEW && status <= DONE) _status = status;
}

int ATask::doTask(void) {
  switch (_status) {
    case NEW:
    case READING:
      collectData();
      break;
    case READY_TO_HANDLE:
    case EXECUTION:
      executeTask();
      break;
    case READY_TO_SEND:
    case SENDING:
      sendResult();
      break;
    default:
      break;
  }
  return 0;
}

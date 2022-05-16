#include "../../../includes/classes/Tasks/ATask.hpp"
#include "../../../includes/classes/Connections/AConnection.hpp"

ATask::ATask(int type, int fd, AConnection* connection)
    : _connection(connection), _isKeepAlive(true), _fd(fd), _status(NEW) {
  switch (type) {
    case UNKNOWN_NETWORK:
    case NETWORK_AUTOINDEX:
    case NETWORK_GET:
    case NETWORK_CGI_PARENT:
    case LOCAL_CGI_INPUT:
    case LOCAL_CGI_OUTPUT:
    case NETWORK_DELETE:
    case NETWORK_POST:
    case NETWORK_HEAD:
    case NETWORK_REDIRECT:
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
bool ATask::getIsKeepAlive(void) const { return _isKeepAlive; }

void ATask::setStatus(int status) {
  if (status > NEW && status <= DONE) _status = status;
}

int ATask::setLastActivity(void) {
  _connection->setLastActivity();
  return 0;
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

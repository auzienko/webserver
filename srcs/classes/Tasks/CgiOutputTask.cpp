#include "../../../includes/classes/Tasks/CgiOutputTask.hpp"
#include "../../../includes/classes/MimeTypes.hpp"

CgiOutputTask::CgiOutputTask(AConnection* connection, int const& fd,
                           int const& parentFd)
    : ATask(NETWORK_CGI_PARENT, fd),
      _connection(connection),
      _parentFd(parentFd) {}

CgiOutputTask::~CgiOutputTask() {}

int CgiOutputTask::collectData(void) { return 0; }

int CgiOutputTask::executeTask(void) { return 0; }

int CgiOutputTask::sendResult(void) { return 0; }

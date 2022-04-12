#include "../../../includes/classes/Tasks/CgiInputTask.hpp"
#include "../../../includes/classes/MimeTypes.hpp"

CgiInputTask::CgiInputTask(AConnection* connection, int const& fd,
                           int const& parentFd)
    : ATask(NETWORK_CGI_PARENT, fd),
      _connection(connection),
      _parentFd(parentFd) {}

CgiInputTask::~CgiInputTask() {}

int CgiInputTask::collectData(void) { return 0; }

int CgiInputTask::executeTask(void) { return 0; }

int CgiInputTask::sendResult(void) { return 0; }

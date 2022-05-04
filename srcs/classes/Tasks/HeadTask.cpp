#include "../../../includes/classes/Tasks/HeadTask.hpp"

HeadTask::HeadTask(AConnection* connection, int const& fd, t_uriInfo parsedURI)
    : GetTask(connection, fd, parsedURI) {}

HeadTask::~HeadTask() {}

int HeadTask::_AssembleResponse(void) {
  _response.clear();
  _response << _Header.str();
  _response << "Content-lenght: " << _Body.str().length() << "\r\n";
  _response << "\r\n";
  setStatus(READY_TO_SEND);
  sendResult();
  return 0;
}

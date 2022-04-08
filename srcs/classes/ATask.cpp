#include "../../includes/classes/ATask.hpp"

ATask::ATask(void) : _status(NEW) {}

ATask::~ATask() {}

int ATask::getStatus(void) const { return _status; }

void ATask::setStatus(int status) {
  if (status > NEW && status <= DONE) _status = status;
}

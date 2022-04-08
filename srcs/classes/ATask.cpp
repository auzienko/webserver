#include "../../includes/classes/ATask.hpp"

ATask::ATask(int type) : _status(NEW) {
  if (type >= 0 && type <= 1) _type = type;
}

ATask::~ATask() {}

int ATask::getStatus(void) const { return _status; }

int ATask::getType(void) const { return _type; }

void ATask::setStatus(int status) {
  if (status > NEW && status <= DONE) _status = status;
}

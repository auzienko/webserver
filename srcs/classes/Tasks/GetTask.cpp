#include "../../../includes/classes/Tasks/GetTask.hpp"
#include "../../../includes/classes/MimeTypes.hpp"

GetTask::GetTask(AConnection* connection, int const& fd, std::string path)
    : ATask(NETWORK_GET, fd), _connection(connection), _path(path) {}

GetTask::~GetTask() {}

int GetTask::collectData(void) { return 0; }

int GetTask::executeTask(void) {
  if (_MakeHeader(_MakeBody()) == 0) _AssembleResponse();
  return 0;
}

int GetTask::_MakeHeader(int status) {
  _Header.clear();
  switch (status) {
    case 404:
      _Header << "HTTP/1.1 404 Not Found\r\n";
      _Header << "Connection: keep-alive\r\n";
      _Header << "Content-type: "
              << "text/html"
              << "\r\n";
      break;
    default:
      _Header << "HTTP/1.1 200 OK\r\n";
      _Header << "Connection: keep-alive\r\n";
      _Header << "Content-type: " << MimeTypes::getMimeType(_path) << "\r\n";
      break;
  }
  return 0;
}

int GetTask::_MakeBody() {
  _Body.clear();
  std::ifstream tmp(_path, std::ifstream::binary);
  if (!tmp.is_open()) {
    std::cout << "Can't GET file " << _path << std::endl;
    _Body << "404";
    return 404;
  }
  _Body << tmp.rdbuf();
  tmp.close();
  return 200;
}

int GetTask::_AssembleResponse(void) {
  _response.clear();
  _response << _Header.str();
  _response << "Content-lenght: " << _Body.str().length() << "\r\n";
  _response << "\r\n";
  _response << _Body.str();
  setStatus(READY_TO_SEND);
  sendResult();
  return 0;
}

int GetTask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  _connection->addToOutput(_response.str());
  setStatus(SENDING);
  return 0;
}

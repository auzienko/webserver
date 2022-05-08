#include "classes/Tasks/GetTask.hpp"
#include "classes/MimeTypes.hpp"
#include "classes/HTTPCodes.hpp"

GetTask::GetTask(AConnection* connection, int const& fd, t_uriInfo parsedURI)
    : ATask(NETWORK_GET, fd), _connection(connection), _parsedURI(parsedURI) {}

GetTask::~GetTask() {}

int GetTask::collectData(void) { return 0; }

int GetTask::executeTask(void) {
  if (_MakeHeader(_MakeBody()) == 0) _AssembleResponse();
  return 0;
}

int GetTask::_MakeHeader(int status) {
  _Header.clear();
  _Header << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(status) << "\r\n";
  _Header << "Connection: keep-alive\r\n";
  _Header << "Content-type: " << _resBodyType << "\r\n";
  return 0;
}

int GetTask::_MakeBody() {
  _Body.clear();
  std::ifstream tmp;
  struct stat  file;
  if (stat(_parsedURI.uri.c_str(), &file)) {
    if (errno == ENOENT) {
      std::cout << "Can't GET file " << _parsedURI.uri << std::endl;
      throw std::logic_error("404");
    }
    else {
      std::cout << "Inprog error" << std::endl;
      return (111);                                     // Внутренняя ошибка
    }
  }
  if (S_ISDIR(file.st_mode) && (!_parsedURI.loc->index.empty() && _parsedURI.loc->index != _parsedURI.uri)) {
    tmp.open(_parsedURI.loc->root + _parsedURI.loc->index, std::ifstream::binary);
    if (!tmp.is_open()) {
      std::cout << "Can't GET file " << _parsedURI.loc->root + _parsedURI.loc->index << std::endl;
      throw std::logic_error("404");
    }
    _resBodyType = MimeTypes::getMimeType(_parsedURI.loc->root + _parsedURI.loc->index);
  } else {
    tmp.open(_parsedURI.uri, std::ifstream::binary);
    if (!tmp.is_open()) {
      std::cout << "Can't GET file " << _parsedURI.uri << std::endl;
      throw std::logic_error("404");
    }
    _resBodyType = MimeTypes::getMimeType(_parsedURI.uri);
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

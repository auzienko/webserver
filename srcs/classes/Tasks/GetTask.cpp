#include "classes/Tasks/GetTask.hpp"
#include "classes/MimeTypes.hpp"
#include "classes/HTTPCodes.hpp"

GetTask::GetTask(AConnection* connection, int const& fd, t_uriInfo parsedURI)
    : ATask(NETWORK_GET, fd, connection), _parsedURI(parsedURI) {}

GetTask::~GetTask() {}

int GetTask::collectData(void) { return 0; }

int GetTask::executeTask(void) {
  if (_MakeHeader(_MakeBody()) == 0) _AssembleResponse();
  return 0;
}

int GetTask::_MakeHeader(int status) {
  _Header.clear();
  _Header << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(status) << "\r\n";
  _Header << "Content-type: " << _resBodyType << "\r\n";
  return 0;
}

int GetTask::_MakeBody() {
  _Body.clear();
  std::ifstream tmp;
  struct stat  file;
  if (stat(_parsedURI.uri.c_str(), &file)) {
    if (errno == ENOENT) {

#ifdef DEBUG
      std::cout << "Can't GET file " << _parsedURI.uri << std::endl;
#endif

      throw std::logic_error("404");
    }
    else {
      return (500);
    }
  }
  // Если ищется папка, есть индекс, попробует открыть индекс-файл
  if (S_ISDIR(file.st_mode) && !_parsedURI.loc->index.empty()) {
    if (*(--_parsedURI.uri.end()) != '/')
      _parsedURI.uri.push_back('/');
    tmp.open(_parsedURI.uri + _parsedURI.loc->index, std::ifstream::binary);
    if (!tmp.is_open()) {

#ifdef DEBUG
      std::cout << "Can't GET file " << _parsedURI.uri + _parsedURI.loc->index << std::endl;
#endif

      throw std::logic_error("404");
    }
    _resBodyType = MimeTypes::getMimeType(_parsedURI.uri + _parsedURI.loc->index);
  } else {
    if (ws::filesIsDir(_parsedURI.uri)) {
      throw std::logic_error("404");
    }
    tmp.open(_parsedURI.uri, std::ifstream::binary);
    if (!tmp.is_open()) {

#ifdef DEBUG
      std::cout << "Can't GET file " << _parsedURI.uri << std::endl;
#endif

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
  _response << "Connection: Close\r\n";
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

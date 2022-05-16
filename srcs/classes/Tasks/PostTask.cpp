#include "../../../includes/classes/Tasks/PostTask.hpp"
#include "../../../includes/classes/MimeTypes.hpp"
#include "../../../includes/classes/HTTPCodes.hpp"

PostTask::PostTask(AConnection* connection, int const& fd, t_uriInfo const& parsedURI, std::string &inputBody)
    : ATask(NETWORK_POST, fd, connection), _parsedURI(parsedURI), _inputBody(inputBody) {}

PostTask::~PostTask() {}

int PostTask::collectData(void) { return 0; }

int PostTask::executeTask(void) {
  if (_MakeHeader() == 0) _AssembleResponse();
  return 0;
}

int PostTask::_MakeHeader() {
  _response.clear();

  std::ofstream outFile;
  struct stat file;
  bool isExists = true;

  // Заполнение информации о файле и проверка на его существование
  if (stat(_parsedURI.uri.c_str(), &file)) {
    if (errno == ENOENT) {
      isExists = false;
    } else {
      throw std::logic_error("500");
    }
  }

  if (isExists) {
    if (S_ISDIR(file.st_mode)) {    // Это папка
      // Создаем новый файл
      outFile.open(_parsedURI.uri);
      outFile << _inputBody;
      outFile.close();
      _resBodyType = "";
    } else {                        // Это файл
      throw std::logic_error("400");
    }
  } else {
    // Создаем новый файл
    outFile.open(_parsedURI.uri);
    outFile << _inputBody;
    outFile.close();
    _resBodyType = "";
  }
  _response << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(201) << "\r\n";
  _response << "Connection: Close\r\n";
  _response << "\r\n";
  return 0;
}

int PostTask::_AssembleResponse(void) {
  setStatus(READY_TO_SEND);
  sendResult();
  return 0;
}

int PostTask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  _connection->addToOutput(_response.str());
  setStatus(SENDING);
  return 0;
}

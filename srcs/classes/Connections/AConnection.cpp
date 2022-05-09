#include "classes/Connections/AConnection.hpp"
#include "classes/MimeTypes.hpp"
#include "classes/HTTPCodes.hpp"

AConnection::AConnection()
    : _connectionManager(nullptr),
      _idFd(-1),
      _task(nullptr) {}

AConnection::~AConnection() {
  killTask();
  close(_idFd);
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🖖 CLOSE FD : "
            << _idFd << std::endl;
}

AConnection::AConnection(ConnectionManager* cm, int fd, const std::map<int, std::string>* error_pages)
    : _connectionManager(cm),
      _idFd(fd),
      _task(nullptr),
      _error_pages(error_pages) {
  setLastActivity();
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🐣 NEW FD : "
            << _idFd << std::endl;
}

void AConnection::setLastActivity(void) { _lastActivity = std::time(0); }

std::time_t AConnection::getLastActivity(void) const { return _lastActivity; }

ConnectionManager* AConnection::getConnectionManager(void) const {
  return _connectionManager;
}

void AConnection::setTask(ATask* task) { _task = task; }

void AConnection::error(const std::exception &ex) {
  int code = ws::stringTakeErrCode(ex.what());
  if (code == -1)
    throw ex;
  else {
    std::cout << "Error " << ex.what() << std::endl;
    std::pair<std::ifstream, std::string> *tmp = ws::filesErrors(code, _error_pages);
    if (!tmp->second.empty()) {
      _output << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(code) << "\r\n";
      // _output << "Connection: keep-alive\r\n";
      _output << "Content-type: " << tmp->second << "\r\n";
      std::stringstream body;
      body << tmp->first.rdbuf();
      tmp->first.close();
      _output << "Content-length: " << body.str().length() << "\r\n";
      _output << "\r\n";
      if (strlen(ex.what()) == 3)
        _output << body.str();
    } else {
      _output << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(code) << "\r\n";
      _output << "Connection: keep-alive\r\n";
      _output << "Content-type: " << MimeTypes::getMimeType("smth.html") << "\r\n";
      std::string text(HTTPCodes::getHTTPCodeString(code));
      _output << "Content-length: " << text.length() << "\r\n";
      _output << "\r\n";
      _output << text;
    }
    _task->setStatus(SENDING);
    delete (tmp);
  }
}

ATask* AConnection::getTask(void) const { return _task; }

void AConnection::killTask(void) {
  if (_task == nullptr) return;
  delete _task;
  _task = nullptr;
}

void AConnection::addToOutput(std::string str) { _output << str; }

int AConnection::getFd(void) { return _idFd; }

std::stringstream& AConnection::getInputData(void) { return _input; }

void AConnection::replaceTask(ATask* newTask) {
  killTask();
  setTask(newTask);
}

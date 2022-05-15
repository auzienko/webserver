#include "classes/Connections/AConnection.hpp"

#include "classes/HTTPCodes.hpp"
#include "classes/MimeTypes.hpp"

AConnection::AConnection()
    : _wrote(0),
      _len(0),
      _connectionManager(nullptr),
      _idFd(-1),
      _task(nullptr) {}

AConnection::~AConnection() {
  killTask();
  close(_idFd);

#ifdef DEBUG
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🖖 CLOSE FD : "
            << _idFd << std::endl;
#endif
}

AConnection::AConnection(ConnectionManager* cm, int fd,
                         const std::map<int, std::string>* error_pages)
    : _wrote(0),
      _len(0),
      _connectionManager(cm),
      _idFd(fd),
      _task(nullptr),
      _error_pages(error_pages) {
  setLastActivity();

#ifdef DEBUG
  std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> 🐣 NEW FD : "
            << _idFd << std::endl;
#endif
}

void AConnection::setLastActivity(void) {
  struct timeval tp;

  gettimeofday(&tp, NULL);
  _lastActivity = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

long int AConnection::getLastActivity(void) const { return _lastActivity; }

ConnectionManager* AConnection::getConnectionManager(void) const {
  return _connectionManager;
}

int AConnection::io() {
  if (!_task) return 0;
  int taskStatus = _task->getStatus();
  switch (taskStatus) {
    case NEW:
      _task->setStatus(READING);
    case READING:
      _reading();
      break;
    case READY_TO_HANDLE:
    case EXECUTION:
      _task->doTask();
      break;
    case READY_TO_SEND:
    case SENDING:
      _writing();
      break;
    case DONE:
      // getConnectionManager()->remove(_idFd);
      break;
    default:
      break;
  }
  return (0);
}

void AConnection::setTask(ATask* task) { _task = task; }

void AConnection::error(const std::exception& ex) {
  int code = ws::stringTakeErrCode(ex.what());
  if (code == -1)
    throw ex;
  else {

#ifdef DEBUG
    std::cout << "Error " << ex.what() << std::endl;
#endif

    std::pair<std::ifstream, std::string>* tmp =
        ws::filesErrors(code, _error_pages);
    if (!tmp->second.empty()) {
      _output << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(code) << "\r\n";
      _output << "Content-type: " << tmp->second << "\r\n";
      std::stringstream body;
      body << tmp->first.rdbuf();
      tmp->first.close();
      _output << "Content-length: " << body.str().length() << "\r\n";
      _output << "Connection: close\r\n\r\n";
      if (strlen(ex.what()) == 3) _output << body.str();
    } else {
      _output << "HTTP/1.1 " << HTTPCodes::getHTTPCodeString(code) << "\r\n";
      _output << "Content-type: " << MimeTypes::getMimeType("smth.html")
              << "\r\n";
      std::string text(HTTPCodes::getHTTPCodeString(code));
      _output << "Content-length: " << text.length() << "\r\n";
      _output << "Connection: close\r\n\r\n";
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

#include "classes/Tasks/AutoindexTask.hpp"

AutoindexTask::AutoindexTask(AConnection* connection, int const& fd,
                             std::string const& realPath, std::string const& publicPath)
    : ATask(NETWORK_AUTOINDEX, fd, connection),
      _realPath(realPath),
      _publicPath(publicPath) {}

AutoindexTask::~AutoindexTask() {}

int AutoindexTask::collectData(void) { return 0; }

int AutoindexTask::executeTask(void) {
  if (_MakeBody() == 0 && _MakeHeader() == 0) _AssembleResponse();
  return 0;
}

int AutoindexTask::_MakeHeader(void) {
  _Header.clear();
  _Header << "HTTP/1.1 200 OK\r\n";
  _Header << "Content-type: text/html\r\n";
  return 0;
}

int AutoindexTask::_MakeBody() {
  _Body.clear();
  std::pair<bool, std::vector<std::string> > content =
      ws::filesReadDirectory(_realPath);
  if (content.first) {
    _Body << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n";
    _Body << "<html><head><title>Index of " << _publicPath
          << "</title></head>\n";
    _Body << "<body><h1>Index of " << _publicPath << "</h1>";
    _Body << "<pre>Name                                      ";
    _Body << "Last modified        ";
    _Body << "Size        ";
    _Body << "Description<hr>";
    std::vector<std::string>::iterator i = content.second.begin();
    std::vector<std::string>::iterator e = content.second.end();
    while (i != e) {
      std::string fileDirName = *i;
      std::string dummy;
      dummy.insert(0, 42 - fileDirName.length(), ' ');
      struct stat st;
      stat((_realPath + fileDirName).c_str(), &st);
      char lastModif[36];
      strftime(lastModif, 36, "%d.%m.%Y %H:%M:%S", localtime(&st.st_mtime));
      size_t size = st.st_size;
      _Body << "<a href=\"" << fileDirName << "\">" << fileDirName << "</a>"
            << dummy << lastModif << "  ";
      if (S_ISDIR(st.st_mode))
        _Body << "-   \n";
      else
        _Body << size << "  \n";
      ++i;
    }
    _Body << "<hr></pre></body></html>";
  } else {
    return -1;
  }
  return 0;
}

int AutoindexTask::_AssembleResponse(void) {
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

int AutoindexTask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  _connection->addToOutput(_response.str());
  setStatus(SENDING);
  return 0;
}

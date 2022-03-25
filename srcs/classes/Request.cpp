#include "../../includes/classes/Request.hpp"


Request::Request(void) : _fd(-1), _status(NEW) {
}

Request::~Request(void) {}

Request::Request(int const& fd) : _fd(fd), _status(NEW) {}

int Request::getFd(void) const { return _fd; }

int Request::getStatus(void) const { return _status; }

void Request::setStatus(int status) { _status = status; }

int Request::getRequest(t_server const& server_config) {
  if (getStatus() >= READY_TO_HANDLE) return 0;

  int nbytes;
  char buf[DEFAULT_BUFLEN];
  int fd = _fd;
  nbytes = recv(fd, buf, DEFAULT_BUFLEN, 0);
  std::cout << "\n\n🤓 Reading " << nbytes << " bytes from socket " << fd << "...\n";
  if (nbytes < 0) {
    //ошибка чтения
    ws::printE("~~ 😞 Server: read failture", "\n");
    return -1;
  } else if (nbytes == 0) {
    ws::print("reading no data", "\n");
    return 0;
  } else {
    int cnt = 0;
    std::cout << "~~ ✔️Server gets " << nbytes << " bytes\nHEADERS:\n";

//header print
    for (int i = 0; i < nbytes && cnt < 2; ++i) {
      write(1, &buf[i], 1);
    }
    write(1, "\n\n", 2);
//end header print

//костыль получения урла
    int j = 0;
    while (buf[j++] != ' ');
    while (buf[j] != ' ') {
      _header.Request_URI += buf[j];
       ++j;
    }
    for (int i = 0; i < nbytes && cnt < 2; ++i) {
      write(1, &buf[i], 1);
    }
//end костыль получения урла

    setStatus(READY_TO_HANDLE);

    if (getStatus() == READY_TO_HANDLE) _RequestHandler(server_config);

    return 0;
  }
}

int Request::_RequestHandler(t_server const& server_config) {
  t_uriInfo cur;

  try
  {
    cur = ConfigUtils::parseURI(_header, server_config);
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  _MakeResponseBody(cur);
  _MakeResponseHeaders(cur);
  _AssembleRespose();
  return 0;
}

// int Request::_MakeResponseBody(t_server const& server_config) {
//   _responseBody.clear();

// //// заглушка на ГЕТ
//   _header.Method = "GET";

//   std::pair<std::string, t_location const*> p =
//       ConfigUtils::getLocationSettings(server_config, _header.Request_URI);
//   if (p.second == nullptr) return -1;

//   // autoindex - сейчас игнорим индекс настройку если есть автоиндекс
// if (_header.Request_URI.at(_header.Request_URI.length() - 1) == '/' && p.second->autoindex){
//     std::cout << "🖖 Autoindex hadler\n";
//     _MakeAutoIndex(_header.Request_URI, p.second->root);
//     return 0;
// }

// if (_header.Request_URI == "/cgi"){
//   _MakeCgiRequest();
//   std::cout << "CGI handled" << std::endl;
//   return 0;
// }

//   std::string url = p.second->root + p.first;
//   if (_header.Method == "GET") {
//     std::ifstream tmp(url, std::ifstream::binary);
//     if (!tmp.is_open()) {
//       std::cout << "Can't GET file " << _header.Request_URI << std::endl;
//       return 404;
//     }
//     _responseBody << tmp.rdbuf();
//     tmp.close();
//   } else if (_header.Method == "POST") {
//   }
//   return 0;
// }

int Request::_MakeResponseBody(t_uriInfo &cur) {
  _responseBody.clear();

//// заглушка на ГЕТ
  _header.Method = "GET";

  if (cur.isCgi)
    _MakeCgiRequest();
  else
  {
    if (!cur.loc)
      throw std::logic_error("Cannt find location");
    else if (cur.loc->autoindex)
      _MakeAutoIndex(cur.uri, cur.loc->root);
    else
      _MakeStdRequest(cur.uri);
  }

  return 0;
}

int Request::_MakeStdRequest(std::string uri) {       // Заглушка из прежней версии
  if (_header.Method == "GET") {
    std::ifstream tmp(uri, std::ifstream::binary);
    if (!tmp.is_open()) {
      std::cout << "Can't GET file " << uri << std::endl;
      return 404;
    }
    _responseBody << tmp.rdbuf();
    tmp.close();
  } else if (_header.Method == "POST") {
  }
  return 0;
}

int Request::_MakeResponseHeaders(t_uriInfo &cur) {
  _responseHeader.clear();
  _responseHeader << "HTTP/1.1 200 OK\r\n";
  _responseHeader << "Connection: keep-alive\r\n";
  _responseHeader << "Content-type: " << MimeTypes::getMimeType(cur.uri) << "\r\n";
  std::cout << "DEBUG! Type: " << MimeTypes::getMimeType(cur.uri) << " for " << cur.uri << std::endl;
  return 0;
}

int Request::_AssembleRespose(void) {
  _response.clear();
  _response << _responseHeader.str();
  _response << "Content-lenght: " << _responseBody.str().length() << "\r\n";
  _response << "\r\n";
  _response << _responseBody.str();
  setStatus(READY_TO_SEND);
  return 0;
}

int Request::_MakeAutoIndex(std::string const& show_path,
                            std::string const& real_path) {
  _responseBody.clear();
  std::pair<bool, std::vector<std::string> > content =
      ws::filesReadDirectory(real_path);
  if (content.first) {
    _responseBody
        << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n";
    _responseBody << "<html><head><title>Index of " << show_path
                  << "</title></head>\n";
    _responseBody << "<body><h1>Index of " << show_path << "</h1>";
    _responseBody << "<pre>Name                                      ";
    _responseBody << "Last modified        ";
    _responseBody << "Size        ";
    _responseBody << "Description<hr>";
    std::vector<std::string>::iterator i = content.second.begin();
    std::vector<std::string>::iterator e = content.second.end();
    while (i != e) {
      std::string file_dir_name = *i;
      std::string dummy;
      dummy.insert(0, 42 - file_dir_name.length(), ' ');
      struct stat st;
      stat((real_path + file_dir_name).c_str(), &st);
      char last_modif[36];
      strftime(last_modif, 36, "%d.%m.%Y %H:%M:%S", localtime(&st.st_mtime));
      size_t size = st.st_size;
      _responseBody << "<a href=\"" << file_dir_name << "\">" << file_dir_name
                    << "</a>" << dummy << last_modif << "  ";
      if (S_ISDIR(st.st_mode))
        _responseBody << "-   \n";
      else
        _responseBody << size << "  \n";
      ++i;
    }
    _responseBody << "<hr></pre></body></html>";
  } else {
    return -1;
  }
  return 0;
}


//https://datatracker.ietf.org/doc/html/rfc3875
//https://firststeps.ru/cgi/cgi1.html
int Request::_MakeCgiRequest(void){
  
  std::cout << "Find CGI!" << std::endl;

  return (0);
}


int Request::sendResult(void) {
  setStatus(SENDING);

  int nbytes, ret;
  ret = 0;
  nbytes = send(_fd, _response.str().c_str(), _response.str().length(), 0);
  if (nbytes < 0) ret = -1;
  printf("Server: write return %d ", ret);
  setStatus(DONE);
  return ret;
}

  // https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
  // https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html
  //необходимо сделать чтение пока не встретим пустую строку (типа гетнекст
  //лайна) далее необходимо сделать разбор заголовка с сложить в структуру
  //t_requestHeader т.е читаем из входящего fd пока не встретим пустую строку,
  //думаю можно побайтово пока не встретим комбинацию \r\n\r\n все что читали
  //куда-то сохраняли то что насохраняли отдаем в парсер, который собирает
  //структуру.

  // GET /hello.htm HTTP/1.1
  // User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
  // Host: www.tutorialspoint.com
  // Accept-Language: en-us
  // Accept-Encoding: gzip, deflate
  // Connection: Keep-Alive

  // POST /cgi-bin/process.cgi HTTP/1.1
  // User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
  // Host: www.tutorialspoint.com
  // Content-Type: application/x-www-form-urlencoded
  // Content-Length: length
  // Accept-Language: en-us
  // Accept-Encoding: gzip, deflate
  // Connection: Keep-Alive
  // r\n\r\n
  // licenseID=string&content=string&/paramsXML=string

  // POST /cgi-bin/process.cgi HTTP/1.1
  // User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
  // Host: www.tutorialspoint.com
  // Content-Type: text/xml; charset=utf-8
  // Content-Length: length
  // Accept-Language: en-us
  // Accept-Encoding: gzip, deflate
  // Connection: Keep-Alive
  // пустая строка
  // <?xml version="1.0" encoding="utf-8"?>
  // <string xmlns="http://clearforest.com/">string</string>

#include "../../includes/classes/Request.hpp"

Request::Request(void) : _fd(-1), _status(NEW) {
}

Request::~Request(void) {}

Request::Request(int const& fd) : _fd(fd), _status(NEW) {}

int Request::getFd(void) const { return _fd; }

int Request::getStatus(void) const { return _status; }

void Request::setStatus(int status) { _status = status; }

int Request::getRequest(void) {
  if (getStatus() >= READY_TO_HANDLE) return 0;

  int nbytes;
  char buf[DEFAULT_BUFLEN];
  int fd = _fd;
  nbytes = recv(fd, buf, DEFAULT_BUFLEN, 0);
  fprintf(stdout, "\n\n🤓 Reading %d bytes from socket %d...\n", nbytes, fd);
  if (nbytes < 0) {
    //ошибка чтения
    perror("~~ 😞 Server: read failture");
    return -1;
  } else if (nbytes == 0) {
    fprintf(stdout, "reading no data\n");
    return 0;
  } else {
    int cnt = 0;
    printf("~~ ✔️Server gets %d bytes\nHEADERS:\n", nbytes);

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

    if (getStatus() == READY_TO_HANDLE) _RequestHandler();

    return 0;
  }
}

int Request::_RequestHandler(void) {
  _MakeResponseBody();
  _MakeResponseHeaders();
  _AssembleRespose();
  return 0;
}

int Request::_MakeResponseBody(void) {
  _responseBody.clear();

  _header.Method = "GET";
 // _header.Request_URI = "/index.html";
  std::string url = "./www-static-site" +  _header.Request_URI;

  if (_header.Method == "GET") {
    std::ifstream tmp(url, std::ifstream::binary);
    if (!tmp.is_open()) {
      std::cout << "Can't GET file " << _header.Request_URI << std::endl;
      return 404;
    }
    _responseBody << tmp.rdbuf();
    tmp.close();
  }
  return 0;
}

int Request::_MakeResponseHeaders(void) {
  _responseHeader.clear();
  _responseHeader << "HTTP/1.1 200 OK\r\n";
  _responseHeader << "Connection: keep-alive\r\n";
  _responseHeader << "Content-type: " << Mime_types::getMimeType(_header.Request_URI) << "\r\n";
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

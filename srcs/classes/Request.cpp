#include "../../includes/classes/Request.hpp"

using namespace std;

Request::Request(void) : _fd(-1), _status(NEW) {  reset();
}

Request::~Request(void) {}

Request::Request(int const& fd) : _fd(fd), _status(NEW) {  reset(); }

int Request::getFd(void) const { return _fd; }

int Request::getStatus(void) const { return _status; }

void Request::setStatus(int status) { _status = status; }

int Request::_RequestHandler(t_server const& server_config) {
  _MakeResponseBody(server_config);
  _MakeResponseHeaders();
  _AssembleRespose();
  return 0;
}

int Request::_MakeResponseBody(t_server const& server_config) {
  _responseBody.clear();
  std::pair<std::string, t_location const*> p =
      ConfigUtils::getLocationSettings(server_config, _request_uri);
  if (p.second == nullptr) return -1;

  // autoindex - сейчас игнорим индекс настройку если есть автоиндекс
if (_request_uri.at(_request_uri.length() - 1) == '/' && p.second->autoindex){
    std::cout << "🖖 Autoindex hadler\n";
    _MakeAutoIndex(_request_uri, p.second->root);
    return 0;
}

  std::string url = p.second->root + p.first;
  if (_method == "GET") {
    std::ifstream tmp(url, std::ifstream::binary);
    if (!tmp.is_open()) {
      std::cout << "Can't GET file " << _request_uri << std::endl;
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
  _responseHeader << "Content-type: " << MimeTypes::getMimeType(_request_uri) << "\r\n";
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

void Request::getSimple(string& body){
    if (_content_len > _client_max_body_size || _body.size() + body.size() > _client_max_body_size)
        throw logic_error(CODE_413);
    _body.append(body);
    body.clear();
    if (_body.length() == _content_len)
        status = END;
}

void Request::getChunked(string& body){
    if (!body.size())
        status = END;
    
    while (status != END){
        int chunkSize;
        size_t i = body.find(CRLF);
        if (i != strnpos){
            stringstream ss;
            ss << std::hex << body.substr(0, i);
            ss >> chunkSize;
            body.erase(0, i + 2);
            if (!chunkSize){
                status = END;
                return ;
            }
            for (int j = 0; j < chunkSize; j++){
                    _body.push_back(body[j]);
            }
            body.erase(0, chunkSize + 2);
            i = body.find(CRLF);
        }
    }
    //status = END;
}

 void Request::parseFirstLine(string& firstLine){
     size_t j = firstLine.find(CRLF);
     if (j == strnpos)
        throw logic_error(CODE_400);
     if (firstLine.size() < 1)
         throw logic_error(CODE_400);
     size_t i = firstLine.find_first_of(" ");
     if (i == strnpos)
         throw logic_error(CODE_400);
     _method = firstLine.substr(0, i);
     firstLine.erase(0, i + 1);
     j = j - i - 1;
     i = firstLine.find(" ");
     if (i == strnpos)
         throw logic_error(CODE_400);
     _request_uri = firstLine.substr(0, i);
     _http_version = firstLine.substr(i + 1, j - i - 1);
     if (_method != GET && _method != POST && _method != DELETE)
         throw logic_error(CODE_501);
     if (_http_version != "HTTP/1.1")
         throw logic_error(CODE_505);
     firstLine.erase(0, j + 2);
     if (firstLine[0] == '\r' && firstLine[1] == '\n' && firstLine[2] == '\0')
      status = END;
     else 
      status = HEADERS;
 }

void Request::parseHeaders(string head){
    string key;
    string value;

    if (head.empty()){
        if (_chunked || _content_len)
            status = BODY;
        else 
            status = END;
        return ;
    }
    size_t i = head.find(":");
    if (i == strnpos)
        throw logic_error(CODE_400);
    key = head.substr(0, i);
    value = head.substr(i + 1);
    _headers.insert(make_pair(key, value));
    if (key == "Content-Length")
        _content_len = stoi(value);
    if (key == "Transfer-Encoding" && value == " chunked")
        _chunked = true;
}

void Request::reset(){
  _method.clear();
  _request_uri.clear();
  _http_version.clear();
  _headers.clear();
  _content_len = 0;
  _client_max_body_size = 0;
  _chunked = false;
  _body.clear();
  status = START;
  _code_status = 0;
}

void Request::parse(char *buf, int nbytes, size_t i){
  std::string tmp;
  buf[nbytes] = 0;
  _client_max_body_size = i;
  tmp.append(buf, nbytes);
  if (status == START)
    parseFirstLine(tmp);
  if (status == HEADERS)
  {
     size_t i = tmp.find(CRLF);
     while (status == HEADERS && i != strnpos){
         parseHeaders(tmp.substr(0, i));
         tmp.erase(0, i + 2);
         i = tmp.find(CRLF);
     }
  }
  if (status == BODY)
  {
    if (_chunked)
        getChunked(tmp);
    if (_content_len)
        getSimple(tmp);
  }
}

void Request::print(){
    cout << "HTTP REQUEST\n" << _method << " " << _request_uri << " " << _http_version << endl;
    for (map<string, string>::iterator it = _headers.begin(); it != _headers.end(); ++it){
      cout << (*it).first << ":" << (*it).second << endl;
    }
    cout << _body << endl;
}

int Request::getRequest(t_server const& server_config) {
  size_t i = server_config.client_max_body_size;
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  int fd = _fd;
  while (1){
    nbytes = recv(fd, &buf, DEFAULT_BUFLEN, 0);
    if (nbytes == -1 && errno == 35)
      break;
    if (nbytes < 0) {
     ws::printE("~~ 😞 Server: read failture", "\n");
     return -1;
    } else if (nbytes == 0) {
      ws::print("reading no data", "\n");
      return 0;
    }
    else
     parse(buf, nbytes, i);
  }
  print();
  setStatus(READY_TO_HANDLE);
  if (getStatus() == READY_TO_HANDLE) _RequestHandler(server_config);
  return 0;
  }

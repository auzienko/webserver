#include "../../includes/classes/Request.hpp"
#include "../../includes/classes/MimeTypes.hpp"
#include "../../includes/classes/ConfigUtils.hpp"
#include "../../includes/classes/LocalConnection.hpp"

using namespace std;

Request::~Request(void) {}

Request::Request(AConnection* connection, int const& fd)
    : ATask(NETWORK), _fd(fd), _parentFd(-1), _connection(connection) {
  reset();
}
Request::Request(AConnection* connection, int const& fd, int const& parentFd)
    : ATask(CGI), _fd(fd), _parentFd(parentFd), _connection(connection) {
  reset();
}

int Request::getFd(void) const { return _fd; }

int Request::_RequestHandler(t_server const& server_config) {
  t_uriInfo cur;

  try
  {
    cur = ConfigUtils::parseURI(_request_uri, server_config);
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  if (_MakeResponseBody(server_config, cur) == 0 &&
      _MakeResponseHeaders(cur) == 0)
    _AssembleRespose();
  return 0;
}

int Request::_MakeResponseBody(t_server const& server_config, t_uriInfo &cur) {
  _responseBody.clear();

  if (cur.isCgi) {
    _MakeCgiRequest(server_config, cur);
    return 1;
  } else {
    if (!cur.loc)
      throw std::logic_error("Cannt find location");
    else if (cur.loc->autoindex)
      _MakeAutoIndex(cur.loc->path, cur.loc->root);
    else
      _MakeStdRequest(cur.uri);
  }
  return 0;
}

int Request::_MakeStdRequest(std::string uri) {       // Заглушка из прежней версии
  if (_method == "GET") {
    std::ifstream tmp(uri, std::ifstream::binary);
    if (!tmp.is_open()) {
      std::cout << "Can't GET file " << uri << std::endl;
      return 404;
    }
    _responseBody << tmp.rdbuf();
    tmp.close();
  } else if (_method== "POST") {
  }
  return 0;
}

int Request::_MakeResponseHeaders(t_uriInfo &cur) {
  _responseHeader.clear();
  _responseHeader << "HTTP/1.1 200 OK\r\n";
  _responseHeader << "Connection: keep-alive\r\n";
  _responseHeader << "Content-type: " << MimeTypes::getMimeType(cur.uri) << "\r\n";
  return 0;
}

int Request::_AssembleRespose(void) {
  _response.clear();
  _response << _responseHeader.str();
  _response << "Content-lenght: " << _responseBody.str().length() << "\r\n";
  _response << "\r\n";
  _response << _responseBody.str();
  setStatus(READY_TO_SEND);
  sendResult();
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
int Request::_MakeCgiRequest(t_server const& server_config, t_uriInfo uriBlocks){
  std::map<std::string, std::string> env;
  env["PATH_INFO"] = uriBlocks.pathInfo;
  env["SERVER_NAME"] = server_config.listen;
  env["AUTH_TYPE"] = ws::stringFromMap(_headers.find("Authorization"), _headers.end());
  env["CONTENT_LENGTH"] = ws::intToStr(_content_len);
  env["GATEWAY_INTERFACE"] = "CGI/1.1";
  env["PATH_TRANSLATED"] = uriBlocks.uri;
  env["CONTENT_TYPE"] =  ws::stringFromMap(_headers.find("Content-Type"), _headers.end());
  env["QUERY_STRING"] = _query;
  env["REMOTE_ADDR"] = ws::socketGetIP(_fd);
  //env["REMOTE_HOST"] = "empty";
  // env["REMOTE_IDENT"] =  remote_ident_pwd;
  // env["REMOTE_USER"] = remote_username;
  env["REQUEST_METHOD"] = _method;
  env["SCRIPT_NAME"] = ws::stringFromMap(server_config.cgi.find("." + ws::stringTail(uriBlocks.uri, '.')), _headers.end());
  env["SERVER_PORT"] = ws::intToStr(server_config.port);
  env["SERVER_PROTOCOL"] = _http_version;
  env["SERVER_SOFTWARE"] = PROGRAMM_NAME;

  std::map<std::string, std::string>::iterator it = _headers.begin();
  std::map<std::string, std::string>::iterator en = _headers.end();
  while (it != en) {
    env["HTTP_" + ws::stringToCGIFormat(it->first)] = it->second;
    ++it;
  }

  //make env char**
  t_z_array zc_env;
  z_array_init(&zc_env);
  std::map<std::string, std::string>::iterator i = env.begin();
  std::map<std::string, std::string>::iterator e = env.end();
  std::string tmp;
  while( i != e){
    if ((*i).second.empty())
      tmp = (*i).first;
    else
      tmp = (*i).first + '=' + (*i).second;
    z_array_append(&zc_env, (char*)tmp.c_str());
    ++i;
  }
  z_array_null_terminate(&zc_env);
  t_z_array zc_cgi_path;
  z_array_init(&zc_cgi_path);
  z_array_append(&zc_cgi_path, (char*)env["SCRIPT_NAME"].c_str());
  z_array_null_terminate(&zc_cgi_path);

  // RUN cgi
  std::cout << "~~~ CGI REQUEST\n";

  pid_t pid;
  int status;
  int fd_input[2];
  int fd_output[2];

  if (pipe(fd_input) < 0) {
    ws::printE(ERROR_CGI_PIPE, "\n");
    return -1;
  }
  if (pipe(fd_output) < 0) {
    ws::printE(ERROR_CGI_PIPE, "\n");
    close(fd_input[0]);
    close(fd_input[1]);
    return -1;
  }
  pid = fork();
  if (pid < 0) {
    ws::printE(ERROR_CGI_EXECVE_FORK, "\n");
    return -1;
  }
  if (pid == 0) {
    if (dup2(fd_input[0], STDIN_FILENO) < 0 ||
        dup2(fd_output[1], STDOUT_FILENO) < 0) {
      ws::printE(ERROR_CGI_DUP2, "\n");
      exit(-1);
    }
    close(fd_input[0]);
    close(fd_input[1]);
    close(fd_output[0]);
    close(fd_output[1]);
    status = execve(zc_cgi_path.elem[0], zc_cgi_path.elem, zc_env.elem);
    ws::printE(ERROR_CGI_EXECVE, "\n");
    exit(status);
  }

  close(fd_input[0]);
  close(fd_output[1]);
  z_array_free(&zc_env);
  z_array_free(&zc_cgi_path);

  // if you need to write something to cgi - use fd_input[1]
  // if you need to read something from cgi - use fd_output[0]

  //удалить это тестовое боди по готовности парсера
  _body = "foo=bar";
  ///
  // _connection->getConnectionManager()->add(fd_output[0], _fd);
  //_connection->getConnectionManager()->add(fd_input[1], _fd);


  // _connection->getConnectionManager()->add(new LocalConnection(
  //     _connection->getConnectionManager(), fd_input[0], _fd));
  // _connection->getConnectionManager()->add(new LocalConnection(
  //     _connection->getConnectionManager(), fd_input[1], fd_input[1]));
  // if (!_body.empty())
  //   _connection->getConnectionManager()
  //       ->at(fd_input[1])
  //       ->getTask()
  //       ->makeResponseFromString(_body);
  // else
  //   _connection->getConnectionManager()
  //       ->at(fd_input[1])
  //       ->getTask()
  //       ->makeResponseFromString("");
  // _connection->getConnectionManager()
  //     ->at(fd_input[1])
  //     ->getTask()
  //     ->setStatus(READY_TO_SEND);
  return 0;
}

int Request::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  _connection->addToOutput(_response.str());
  setStatus(SENDING);
  return 0;
}

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
  if (_parentFd == -1) {
    cout << "HTTP REQUEST (fd: " << _fd << ")\n"
         << _method << " " << _request_uri << " " << _http_version << endl;
    for (map<string, string>::iterator it = _headers.begin();
         it != _headers.end(); ++it) {
      cout << (*it).first << ":" << (*it).second << endl;
    }
    cout << _body << endl;
  }
}

int Request::getRequest(t_server const& server_config) {
  if (getStatus() < READY_TO_HANDLE)
    setStatus(READING);
  else
    return 0;
  size_t i = server_config.client_max_body_size;
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  int fd = _fd;
  while (1){
    nbytes = read(fd, &buf, DEFAULT_BUFLEN);
    if (nbytes == -1)
      break;
    if (nbytes < 0) {
     ws::printE("~~ 😞 Server: read failture", "\n");
     return -1;
    } else if (nbytes == 0) {
      std::cout << "fd: " << _fd << " reading no data\n";
      return 0;
    } else {
      if (getType() == CGI) {
        //тестовый вывод cgi
        printf("cgi request body");
        write(2, buf, nbytes);
        _connection->getConnectionManager()->remove(fd);
        // status = HEADERS;
        // parse(buf, nbytes, i);
        return 0;
      } else {
        //тут надо подумать, почему только buf берется? (Рита, надо подумать
        //тут)
        parse(buf, nbytes, i);
        print();
        //проставить этот статус после успешного парсинга
        setStatus(READY_TO_HANDLE);
        _RequestHandler(server_config);
      }
    }
  }
  return 0;
  }

  int Request::makeResponseFromString(std::string str) {
    _response << str;
    return 0;
  }

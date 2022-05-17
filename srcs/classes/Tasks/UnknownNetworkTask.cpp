#include "classes/Tasks/UnknownNetworkTask.hpp"

#include "classes/ConfigUtils.hpp"
#include "classes/Connections/LocalConnection.hpp"
#include "classes/MimeTypes.hpp"
#include "classes/Tasks/AutoindexTask.hpp"
#include "classes/Tasks/CgiInputTask.hpp"
#include "classes/Tasks/CgiOutputTask.hpp"
#include "classes/Tasks/CgiParentTask.hpp"
#include "classes/Tasks/DeleteTask.hpp"
#include "classes/Tasks/GetTask.hpp"
#include "classes/Tasks/HeadTask.hpp"
#include "classes/Tasks/PostTask.hpp"
#include "classes/Tasks/RedirTask.hpp"

using namespace std;

UnknownNetworkTask::~UnknownNetworkTask(void) {}

UnknownNetworkTask::UnknownNetworkTask(AConnection* connection, int const& fd)
    : ATask(UNKNOWN_NETWORK, fd, connection),
      _server_config(connection->getConnectionManager()
                         ->getWebserver()
                         ->getServerConfig()) {
  k = 0;
  reset();
}

int UnknownNetworkTask::collectData(void) {
  if (getStatus() < READY_TO_HANDLE) setStatus(READING);

  parse(_connection->getInputData());
  if (!_done) return 0;

  print();
  setStatus(READY_TO_HANDLE);
  executeTask();
  return 0;
}

int UnknownNetworkTask::executeTask(void) {
  t_uriInfo cur;

  cur = ConfigUtils::parseURI(_UnknownNetworkTask_uri, _server_config, _method);
  return _MakeKnownTask(cur);
}

int UnknownNetworkTask::sendResult(void) { return 0; }

int UnknownNetworkTask::_MakeKnownTask(t_uriInfo& cur) {
  if (cur.isCgi) {
    if (cur.cgi_methods.find(_method) != cur.cgi_methods.end() ||
        cur.cgi_methods.empty()) {
      std::cout << "~~~~~~~~~~~~~~~> CREATE CGI TASK uri '"
                << _UnknownNetworkTask_uri << "' \n\n";
      _MakeCgiTasks(_server_config, cur);
    } else
      throw std::logic_error(_method == "HEAD" ? "405h" : "405");
    return 42;
  } else {
    if (!cur.loc)
      throw std::logic_error("404");
    else if (cur.loc->client_max_body_size &&
             _body.length() > cur.loc->client_max_body_size)
      throw std::logic_error("413");
    else if (cur.loc->redir.code != 0) {
      std::cout << "~~~~~~~~~~~~~~~> CREATE REDIR TASK uri '"
                << _UnknownNetworkTask_uri << "' \n\n";
      RedirTask* tmp = new RedirTask(_connection, getFd(), cur);
      tmp->setStatus(READY_TO_HANDLE);
      _connection->replaceTask(tmp);
      return 42;
    } else if (cur.loc->autoindex) {
      // Autoindex flow

      std::cout << "~~~~~~~~~~~~~~~> CREATE AUTOINDEX TASK uri '"
                << _UnknownNetworkTask_uri << "' \n\n";
      if (cur.loc->root == cur.uri && ws::filesIsDir(cur.uri)) {
        AutoindexTask* tmp = new AutoindexTask(_connection, getFd(),
                                               cur.loc->root, cur.loc->path);
        tmp->setStatus(READY_TO_HANDLE);
        _connection->replaceTask(tmp);
        return 42;
      } else
        throw std::logic_error("404");
    } else if (_method == "GET") {
      // GET flow

      std::cout << "~~~~~~~~~~~~~~~> CREATE GET TASK uri '"
                << _UnknownNetworkTask_uri << "' \n\n";
      GetTask* tmp = new GetTask(_connection, getFd(), cur);
      tmp->setStatus(READY_TO_HANDLE);
      _connection->replaceTask(tmp);
      return 42;
    } else if (_method == "POST") {
      // POST flow

      std::cout << "~~~~~~~~~~~~~~~> CREATE POST TASK uri '"
                << _UnknownNetworkTask_uri << "' \n\n";
      PostTask* tmp = new PostTask(_connection, getFd(), cur, _body);
      tmp->setStatus(READY_TO_HANDLE);
      _connection->replaceTask(tmp);
      return 42;
    } else if (_method == "DELETE") {
      // DELETE flow

      std::cout << "~~~~~~~~~~~~~~~> CREATE DELETE TASK uri '"
                << _UnknownNetworkTask_uri << "' \n\n";
      DeleteTask* tmp = new DeleteTask(_connection, getFd(), cur);
      tmp->setStatus(READY_TO_HANDLE);
      _connection->replaceTask(tmp);
      return 42;
    } else if (_method == "HEAD") {
      // HEAD only for tester flow

      std::cout << "~~~~~~~~~~~~~~~> CREATE HEAD TASK uri '"
                << _UnknownNetworkTask_uri << "' \n\n";
      HeadTask* tmp = new HeadTask(_connection, getFd(), cur);
      tmp->setStatus(READY_TO_HANDLE);
      _connection->replaceTask(tmp);
      return 42;
    } else if (_method == "PUT") {
      // PUT only for tester flow

      std::cout << "~~~~~~~~~~~~~~~> CREATE PUT TASK uri '"
                << _UnknownNetworkTask_uri << "' \n\n";

      std::string response("HTTP/1.1 201 Created\r\nConnection: Close\r\n\r\n");
      _connection->addToOutput(response);
      setStatus(SENDING);
      return 42;
    }
  }
  throw std::logic_error("501");
}

void UnknownNetworkTask::getSimple(string& body) {
  if (_content_len < 0) throw logic_error("400"); // тут проблема, тип ансайн проверяется на меньше нуля
  if (_client_max_body_size &&
      (_content_len > _client_max_body_size ||
       _body.size() + body.size() > _client_max_body_size))
    throw logic_error("413");
  _body.append(body);
  body.clear();
  if (_body.length() == _content_len) status = END;
}

void UnknownNetworkTask::getChunked(string& body) {
  if (!body.size()) return;  // Если боди нет, возврат на чтение
  if (_client_max_body_size &&
      _body.length() + body.length() > _client_max_body_size)
    throw logic_error("413");

  while (status != END) {
    if (body[0] == '\r' || body[0] == '\n') {
      body.erase(0, 1);
      if (body.length() && body[1] == '\n') body.erase(0, 1);
    }
    size_t l = body.find(CRLF);
    if (!_chunkSize) {
      if (l != strnpos) {
        stringstream ss;
        ss << std::hex << body.substr(0, l);
        ss >> _chunkSize;
        body.erase(0, l + 2);
      } else
        return;  // Дочитываем, если чанксайз 0 (дефолтное значение) и рн не
                 // найден
      if (!_chunkSize && l != strnpos) {  // Чанксайз все еще 0 и рн был, значит
                                          // чанксайз пришел 0
        status = END;
        return;
      }
    }
    std::string::size_type i = 0;
    while (k < static_cast<unsigned long int>(_chunkSize) &&
           i < static_cast<unsigned long int>(body.length())) {
      _body.push_back(body[i]);
      i++;
      k++;
    }
    if (k < static_cast<unsigned long int>(
                _chunkSize)) {  // Прочитано меньше размера чанка
      body.erase(0, i);
      return;
    }
    body.erase(0, i + 2);
    if (k == static_cast<unsigned long int>(_chunkSize)) {
      _chunkSize = 0;
      k = 0;
    }
    if (!body.size()) return;
  }
  return;
}

void UnknownNetworkTask::parseFirstLine(string& firstLine) {
  size_t j = firstLine.find(CRLF);
  if (firstLine.size() < 1) throw logic_error("400");
  size_t i = firstLine.find_first_of(" ");
  if (i == strnpos) throw logic_error("400");
  _method = firstLine.substr(0, i);
  firstLine.erase(0, i + 1);
  j = j - i - 1;
  i = firstLine.find(" ");
  if (i == strnpos) throw logic_error("400");
  _UnknownNetworkTask_uri = firstLine.substr(0, i);
  _http_version = firstLine.substr(i + 1, j - i - 1);
  if (_method != GET && _method != POST && _method != "PUT" &&
      _method != DELETE && _method != HEAD)
    throw logic_error("501");
  if (_http_version != "HTTP/1.1") throw logic_error("505");
  firstLine.erase(0, j + 2);
  if (firstLine[0] == '\r' && firstLine[1] == '\n' && firstLine[2] == '\0')
    status = END;
  else
    status = HEADERS;
}

void UnknownNetworkTask::parseHeaders(string head) {
  string key;
  string value;

  size_t i = head.find(":");
  if (i == strnpos) throw logic_error("400");
  key = head.substr(0, i);
  try {
    head.at(i + 2); // NOT USED?
    value = head.substr(i + 2);  // TRIM?
  } catch (...) {
    value = "";
  }
  _headers.insert(make_pair(key, value));
  if (key == "Content-Length") _content_len = stoi(value);
  if (key == "Transfer-Encoding") {
    if (value == "chunked")
      _chunked = true;
    else
      throw logic_error("400");
  }
}

void UnknownNetworkTask::reset() {
  _method.clear();
  _UnknownNetworkTask_uri.clear();
  _http_version.clear();
  _headers.clear();
  _content_len = 0;
  _client_max_body_size = _server_config.client_max_body_size;
  _chunked = false;
  _body.clear();
  status = START;
  _code_status = 0;
  _done = false;
  _chunkSize = 0;
}

void UnknownNetworkTask::parse(std::stringstream& str) {
  std::string tmp1 = str.str();
  std::string tmp = _read;
  tmp.append(tmp1);

  if (status == END && _done) reset();
  _client_max_body_size = _server_config.client_max_body_size;
  size_t j = tmp.find(CRLF);
  if (status == START && j != strnpos) parseFirstLine(tmp);
  if (status == HEADERS) {
    size_t i = tmp.find(CRLF);
    while (status == HEADERS && i != strnpos) {
      if (i == 0) {  // true когда пустая строка с /r/n
        if (_chunked || _content_len)
          status = BODY;
        else
          status = END;
        if (_content_len > 0 && _chunked) throw logic_error("400");
        if (_headers.find("Host") == std::end(_headers))
          throw logic_error("400");
        tmp.erase(i, i + 2);
      } else {
        parseHeaders(tmp.substr(0, i));
        tmp.erase(0, i + 2);
        i = tmp.find(CRLF);
      }
    }
  }
  if (status == BODY) {
    if (_chunked) getChunked(tmp);
    if (_content_len) getSimple(tmp);
  }
  if (status == END) {
    if (_chunked) {
      if (tmp == CRLF) {
        _done = true;
        tmp.clear();
      }
    } else
      _done = true;
  }
  _read = tmp;
}

void UnknownNetworkTask::print() {
  cout << "HTTP REQUEST (fd: " << getFd() << ")\n"
       << _method << " " << _UnknownNetworkTask_uri << " " << _http_version
       << endl;
  for (map<string, string>::iterator it = _headers.begin();
       it != _headers.end(); ++it) {
    cout << (*it).first << ":" << (*it).second << endl;
  }
  // cout << _body << endl;  //Артём это почему так?
}

int UnknownNetworkTask::_MakeCgiTasks(t_server const& server_config,
                                      t_uriInfo uriBlocks) {
  std::map<std::string, std::string> env;
  env["PATH_INFO"] = uriBlocks.uri;
  env["REQUEST_URI"] = uriBlocks.uri;
  env["SERVER_NAME"] = server_config.listen;
  env["AUTH_TYPE"] =
      ws::stringFromMap(_headers.find("Authorization"), _headers.end());
  env["CONTENT_LENGTH"] = ws::intToStr(_content_len);
  env["GATEWAY_INTERFACE"] = "CGI/1.1";
  env["PATH_TRANSLATED"] = uriBlocks.uri;
  env["CONTENT_TYPE"] =
      ws::stringFromMap(_headers.find("Content-Type"), _headers.end());
  env["QUERY_STRING"] = uriBlocks.args;
  env["REMOTE_ADDR"] = ws::socketGetIP(getFd());
  env["REQUEST_METHOD"] = _method;
  env["SCRIPT_NAME"] = ws::stringFromMap(
      server_config.cgi.find("." + ws::stringTail(uriBlocks.uri, '.')),
      server_config.cgi.end());
  env["SERVER_PORT"] = ws::intToStr(server_config.port);
  env["SERVER_PROTOCOL"] = _http_version;
  env["SERVER_SOFTWARE"] = PROGRAMM_NAME;
  std::map<std::string, std::string>::iterator it = _headers.begin();
  std::map<std::string, std::string>::iterator en = _headers.end();
  while (it != en) {
    env["HTTP_" + ws::stringToCGIFormat(it->first)] = it->second;
    ++it;
  }
  t_z_array zc_env;
  z_array_init(&zc_env);
  std::map<std::string, std::string>::iterator i = env.begin();
  std::map<std::string, std::string>::iterator e = env.end();
  std::string tmp;
  while (i != e) {
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
  char* tmpURI = strdup(uriBlocks.uri.c_str());
  z_array_append(&zc_cgi_path, tmpURI);
  free(tmpURI);
  z_array_null_terminate(&zc_cgi_path);

  pid_t pid;
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
  if (fcntl(fd_input[0], F_SETFL, O_NONBLOCK) < 0 ||
      fcntl(fd_input[1], F_SETFL, O_NONBLOCK) < 0 ||
      fcntl(fd_output[0], F_SETFL, O_NONBLOCK) < 0 ||
      fcntl(fd_output[1], F_SETFL, O_NONBLOCK) < 0) {
    ws::printE(ERROR_CGI_PIPE, "\n");
    close(fd_input[0]);
    close(fd_input[1]);
    close(fd_output[0]);
    close(fd_output[1]);
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
    execve(zc_cgi_path.elem[0], zc_cgi_path.elem, zc_env.elem);
    ws::printE(ERROR_CGI_EXECVE, "\n");
    exit(status);
  }
  close(fd_input[0]);
  close(fd_output[1]);
  z_array_free(&zc_env);
  z_array_free(&zc_cgi_path);
  LocalConnection* tmpConnectionInput =
      new LocalConnection(_connection->getConnectionManager(), fd_input[1],
                          &_server_config.error_pages);
  tmpConnectionInput->addToOutput(_body);
  LocalConnection* tmpConnectionOutput =
      new LocalConnection(_connection->getConnectionManager(), fd_output[0],
                          &_server_config.error_pages);
  CgiParentTask* tmpParent =
      new CgiParentTask(_connection, getFd(), fd_input[1], fd_output[0], pid);
  CgiInputTask* tmpInput =
      new CgiInputTask(tmpConnectionInput, fd_input[1], getFd());
  CgiOutputTask* tmpOutput =
      new CgiOutputTask(tmpConnectionOutput, fd_output[0], getFd());
  tmpParent->setStatus(READY_TO_HANDLE);
  tmpInput->setStatus(READY_TO_SEND);
  tmpOutput->setStatus(NEW);
  tmpConnectionInput->setTask(tmpInput);
  tmpConnectionOutput->setTask(tmpOutput);
  _connection->getConnectionManager()->add(tmpConnectionInput);
  _connection->getConnectionManager()->add(tmpConnectionOutput);
  _connection->replaceTask(tmpParent);
  return 0;
}

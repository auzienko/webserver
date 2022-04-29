#include "../../../includes/classes/Tasks/UnknownNetworkTask.hpp"
#include "../../../includes/classes/Tasks/AutoindexTask.hpp"
#include "../../../includes/classes/Tasks/GetTask.hpp"
#include "../../../includes/classes/Tasks/RedirTask.hpp"
#include "../../../includes/classes/Tasks/CgiParentTask.hpp"
#include "../../../includes/classes/Tasks/CgiInputTask.hpp"
#include "../../../includes/classes/Tasks/CgiOutputTask.hpp"
#include "../../../includes/classes/ConfigUtils.hpp"
#include "../../../includes/classes/Connections/LocalConnection.hpp"
#include "../../../includes/classes/MimeTypes.hpp"

using namespace std;

UnknownNetworkTask::~UnknownNetworkTask(void) {}

UnknownNetworkTask::UnknownNetworkTask(AConnection* connection,
                                       t_server const& server_config,
                                       int const& fd)
    : ATask(UNKNOWN_NETWORK, fd),
      _connection(connection),
      _server_config(server_config) {
  reset();
}

int UnknownNetworkTask::collectData(void) {
  if (getStatus() < READY_TO_HANDLE) setStatus(READING);

  parse(_connection->getInputData());
  print();
  //проставить этот статус после успешного парсинга
  setStatus(READY_TO_HANDLE);
  executeTask();
  return 0;
}

int UnknownNetworkTask::executeTask(void) {
  t_uriInfo cur;

  try {
    cur = ConfigUtils::parseURI(_UnknownNetworkTask_uri, _server_config, _method);
  } catch (std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }
  return _MakeKnownTask(cur);
}

int UnknownNetworkTask::sendResult(void) {
  return 0;
}

int UnknownNetworkTask::_MakeKnownTask(t_uriInfo& cur) {
  if (cur.isCgi) {
    if (cur.cgi_methods.find(_method) != cur.cgi_methods.end() || cur.cgi_methods.empty())
      _MakeCgiTasks(_server_config, cur);
    else
      throw std::logic_error("CGI wrong method");             //Добавить нормальный обработчик ошибок (сега деструкторов)
    return 42;
  } else {
    if (!cur.loc)
      throw std::logic_error("Cannt find location");
    else if (cur.loc->redir.code != 0) {
      std::cout << "~~~~~~~~~~~~~~~> CREATE REDIR TASK uri '" << _UnknownNetworkTask_uri << "' \n\n";
      RedirTask* tmp = new RedirTask(_connection, getFd(), cur);
      tmp->setStatus(READY_TO_HANDLE);
      _connection->replaceTask(tmp);
      return 42;
    } else if (cur.loc->autoindex) {
      // start autoindex flow
      std::cout << "~~~~~~~~~~~~~~~> CREATE AUTOINDEX TASK uri '" << _UnknownNetworkTask_uri << "' \n\n";
      if (cur.loc->root == cur.uri && ws::filesIsDir(cur.uri)) {
        AutoindexTask* tmp =
            new AutoindexTask(_connection, getFd(), cur.loc->root, cur.loc->path);
        tmp->setStatus(READY_TO_HANDLE);
        _connection->replaceTask(tmp);
        return 42;
      } else
        throw std::logic_error("404");                        //Добавить нормальный обработчик ошибок (сега деструкторов)
    } else if (_method == "GET") {
      // GET flow
      std::cout << "~~~~~~~~~~~~~~~> CREATE GET TASK uri '" << _UnknownNetworkTask_uri << "' \n\n";
      GetTask* tmp = new GetTask(_connection, getFd(), cur);
      tmp->setStatus(READY_TO_HANDLE);
      _connection->replaceTask(tmp);
      return 42;
    }
  }
  throw std::logic_error("Wrong method (mb 404)");             //Добавить нормальный обработчик ошибок (сега деструкторов)
  return 0;
}

void UnknownNetworkTask::getSimple(string& body) {
  if (_content_len < 0) throw logic_error(CODE_400);
  if (_content_len > _client_max_body_size ||
      _body.size() + body.size() > _client_max_body_size)
    throw logic_error(CODE_413);
  _body.append(body);
  body.clear();
  if (_body.length() == _content_len) status = END;
}

void UnknownNetworkTask::getChunked(string& body){
  if (!body.size()) status = END;

  while (status != END)
  {
     size_t i = body.find(CRLF);
     if (!_chunkSize){
        if (i != strnpos) {
          stringstream ss;
          ss << std::hex << body.substr(0, i);
          ss >> _chunkSize;
          body.erase(0, i + 2);
        }
    }
    if (!_chunkSize){
        status = END;
        return;
    }
    std::string::size_type k = -1;
    int j = -1;
    while (++k < body.length() && ++j < _chunkSize){
        _body.push_back(body[j]);
        _chunkSize--;
    }
    body.erase(0, j + 2);
    if (!body.size()) return;
  }
}

void UnknownNetworkTask::parseFirstLine(string& firstLine) {
  size_t j = firstLine.find(CRLF);
  if (j == strnpos) throw logic_error(CODE_400);
  if (firstLine.size() < 1) throw logic_error(CODE_400);
  size_t i = firstLine.find_first_of(" ");
  if (i == strnpos) throw logic_error(CODE_400);
  _method = firstLine.substr(0, i);
  firstLine.erase(0, i + 1);
  j = j - i - 1;
  i = firstLine.find(" ");
  if (i == strnpos) throw logic_error(CODE_400);
  _UnknownNetworkTask_uri = firstLine.substr(0, i);
  _http_version = firstLine.substr(i + 1, j - i - 1);
  if (_method != GET && _method != POST && _method != DELETE)
    throw logic_error(CODE_501);
  if (_http_version != "HTTP/1.1") throw logic_error(CODE_505);
  firstLine.erase(0, j + 2);
  if (firstLine[0] == '\r' && firstLine[1] == '\n' && firstLine[2] == '\0')
    status = END;
  else
    status = HEADERS;
}

void UnknownNetworkTask::parseHeaders(string head) {
  string key;
  string value;

  if (head.empty()) {
    if (_content_len > 0 && _chunked) throw logic_error(CODE_400);
    if (_headers.find("Host") == std::end(_headers)) throw logic_error(CODE_400);
    if (_chunked || _content_len)
      status = BODY;
    else
      status = END;
    return;
  }
  size_t i = head.find(":");
  if (i == strnpos) throw logic_error(CODE_400);
  key = head.substr(0, i);
  value = head.substr(i + 1);
  _headers.insert(make_pair(key, value));
  if (key == "Content-Length") _content_len = stoi(value);
  if (key == "Transfer-Encoding"){
    if (value == " chunked")
      _chunked = true;
    else 
      throw logic_error(CODE_400);
  }
}

void UnknownNetworkTask::reset() {
  _method.clear();
  _UnknownNetworkTask_uri.clear();
  _http_version.clear();
  _headers.clear();
  _content_len = 0;
  _client_max_body_size = 0;
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
  
  if (status == END) reset();
  _client_max_body_size = _server_config.client_max_body_size;
  if (status == START) parseFirstLine(tmp);
  if (status == HEADERS) {
    size_t i = tmp.find(CRLF);
    while (status == HEADERS && i != strnpos) {
      parseHeaders(tmp.substr(0, i));
      tmp.erase(0, i + 2);
      i = tmp.find(CRLF);
    }
  }
  if (status == BODY) {
    if (_chunked) getChunked(tmp);
    if (_content_len) getSimple(tmp);
  }
  _read = tmp;
  if (status == END) _done = true;
}

void UnknownNetworkTask::print() {
  cout << "HTTP REQUEST (fd: " << getFd() << ")\n"
       << _method << " " << _UnknownNetworkTask_uri << " " << _http_version
       << endl;
  for (map<string, string>::iterator it = _headers.begin();
       it != _headers.end(); ++it) {
    cout << (*it).first << ":" << (*it).second << endl;
  }
  cout << _body << endl;
}

int UnknownNetworkTask::_MakeCgiTasks(t_server const& server_config, t_uriInfo uriBlocks){
  std::map<std::string, std::string> env;
  env["PATH_INFO"] = uriBlocks.pathInfo;
  env["SERVER_NAME"] = server_config.listen;
  env["AUTH_TYPE"] = ws::stringFromMap(_headers.find("Authorization"), _headers.end());
  env["CONTENT_LENGTH"] = ws::intToStr(_content_len);
  env["GATEWAY_INTERFACE"] = "CGI/1.1";
  env["PATH_TRANSLATED"] = uriBlocks.uri;
  env["CONTENT_TYPE"] =  ws::stringFromMap(_headers.find("Content-Type"), _headers.end());
  env["QUERY_STRING"] = _query;
  env["REMOTE_ADDR"] = ws::socketGetIP(getFd());
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
    std::cerr << zc_cgi_path.elem[0] << std::endl;
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

  std::cout << "~~~ CREATE CGI TASKs\n";

  LocalConnection* tmpConnectionInput = new LocalConnection(_connection->getConnectionManager(), fd_input[1]);
  tmpConnectionInput->addToOutput(_body);
  LocalConnection* tmpConnectionOutput = new LocalConnection(_connection->getConnectionManager(), fd_output[0]);
  CgiParentTask* tmpParent = new CgiParentTask(_connection, getFd(), fd_input[1], fd_output[0]);
  CgiInputTask* tmpInput = new CgiInputTask(tmpConnectionInput, fd_input[1], getFd());
  CgiOutputTask* tmpOutput = new CgiOutputTask(tmpConnectionOutput, fd_output[0], getFd());
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

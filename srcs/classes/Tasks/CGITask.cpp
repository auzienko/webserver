#include "../../../includes/classes/Tasks/CGITask.hpp"
#include "../../../includes/classes/MimeTypes.hpp"
#include "../../../includes/classes/ConfigUtils.hpp"

CGITask::~CGITask(){

}

int CGITask::sendResult(void) {
  if (getStatus() != READY_TO_SEND) return -1;
  _connection->addToOutput(_response.str());
  setStatus(SENDING);
  return 0;
}

int CGITask::Build(int fd, t_server const& server_config, t_uriInfo &cur){
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


  _connection->getConnectionManager()->add(new LocalConnection(
      _connection->getConnectionManager(), fd_input[0], _fd));
  _connection->getConnectionManager()->add(new LocalConnection(
      _connection->getConnectionManager(), fd_input[1], fd_input[1]));
  if (!_body.empty())
    _connection->getConnectionManager()
        ->at(fd_input[1])
        ->getTask()
        ->makeResponseFromString(_body);
  else
    _connection->getConnectionManager()
        ->at(fd_input[1])
        ->getTask()
        ->makeResponseFromString("");
  _connection->getConnectionManager()
      ->at(fd_input[1])
      ->getTask()
      ->setStatus(READY_TO_SEND);
  return 0;
}

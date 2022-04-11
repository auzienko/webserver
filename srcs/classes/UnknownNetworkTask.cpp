#include "../../includes/classes/UnknownNetworkTask.hpp"
#include "../../includes/classes/AutoindexTask.hpp"
#include "../../includes/classes/ConfigUtils.hpp"
#include "../../includes/classes/LocalConnection.hpp"
#include "../../includes/classes/MimeTypes.hpp"

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
  return _UnknownNetworkTaskHandler();
}

int UnknownNetworkTask::sendResult(void) {
  setStatus(SENDING);
  return 0;
}

int UnknownNetworkTask::_UnknownNetworkTaskHandler(void) {
  t_uriInfo cur;

  try {
    cur = ConfigUtils::parseURI(_UnknownNetworkTask_uri, _server_config);
  } catch (std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }
  if (_MakeResponseBody(cur) == 0 && _MakeResponseHeaders(cur) == 0)
    _AssembleResponse();
  return 0;
}

int UnknownNetworkTask::_MakeResponseBody(t_uriInfo& cur) {
  _responseBody.clear();

  if (cur.isCgi) {
    //   _MakeCgiUnknownNetworkTask(_server_config, cur);
    return 1;
  } else {
    if (!cur.loc)
      throw std::logic_error("Cannt find location");
    else if (cur.loc->autoindex) {
      // start autoindex flow
      AutoindexTask* tmp =
          new AutoindexTask(_connection, getFd(), cur.loc->root, cur.loc->path);
      tmp->setStatus(READY_TO_HANDLE);
      _connection->replaceTask(tmp);
      return 42;
    } else
      _MakeStdUnknownNetworkTask(cur.uri);
  }
  return 0;
}

int UnknownNetworkTask::_MakeStdUnknownNetworkTask(
    std::string uri) {  // Заглушка из прежней версии
  if (_method == "GET") {
    std::ifstream tmp(uri, std::ifstream::binary);
    if (!tmp.is_open()) {
      std::cout << "Can't GET file " << uri << std::endl;
      return 404;
    }
    _responseBody << tmp.rdbuf();
    tmp.close();
  } else if (_method == "POST") {
  }
  return 0;
}

int UnknownNetworkTask::_MakeResponseHeaders(t_uriInfo& cur) {
  _responseHeader.clear();
  _responseHeader << "HTTP/1.1 200 OK\r\n";
  _responseHeader << "Connection: keep-alive\r\n";
  _responseHeader << "Content-type: " << MimeTypes::getMimeType(cur.uri)
                  << "\r\n";
  return 0;
}

int UnknownNetworkTask::_AssembleResponse(void) {
  std::stringstream response;
  response.clear();
  response << _responseHeader.str();
  response << "Content-lenght: " << _responseBody.str().length() << "\r\n";
  response << "\r\n";
  response << _responseBody.str();
  _connection->addToOutput(response.str());
  sendResult();
  return 0;
}

void UnknownNetworkTask::getSimple(string& body) {
  if (_content_len > _client_max_body_size ||
      _body.size() + body.size() > _client_max_body_size)
    throw logic_error(CODE_413);
  _body.append(body);
  body.clear();
  if (_body.length() == _content_len) status = END;
}

void UnknownNetworkTask::getChunked(string& body) {
  if (!body.size()) status = END;

  while (status != END) {
    int chunkSize;
    size_t i = body.find(CRLF);
    if (i != strnpos) {
      stringstream ss;
      ss << std::hex << body.substr(0, i);
      ss >> chunkSize;
      body.erase(0, i + 2);
      if (!chunkSize) {
        status = END;
        return;
      }
      for (int j = 0; j < chunkSize; j++) {
        _body.push_back(body[j]);
      }
      body.erase(0, chunkSize + 2);
      i = body.find(CRLF);
    }
  }
  // status = END;
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
  if (key == "Transfer-Encoding" && value == " chunked") _chunked = true;
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
}

void UnknownNetworkTask::parse(char* buf, int nbytes, size_t i) {
  std::string tmp;
  buf[nbytes] = 0;
  _client_max_body_size = i;
  tmp.append(buf, nbytes);
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
}

void UnknownNetworkTask::parse(std::stringstream& str) {
  std::string tmp = str.str();
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

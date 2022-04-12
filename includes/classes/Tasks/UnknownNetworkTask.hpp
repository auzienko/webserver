#ifndef UNKNOWNNETWORKTAST_HPP
#define UNKNOWNNETWORKTAST_HPP

#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"
#define CRLF "\r\n"
#define strnpos std::string::npos

#include "../Connections/AConnection.hpp"
#include "ATask.hpp"

class AConnection;

using namespace std;

enum States { START, HEADERS, BODY, END };

class UnknownNetworkTask : public ATask {
 private:
  AConnection* _connection;
  t_server const& _server_config;

  UnknownNetworkTask(void);
  UnknownNetworkTask(UnknownNetworkTask const& src);
  UnknownNetworkTask& operator=(UnknownNetworkTask const& rhs);
  int _MakeKnownTask(t_uriInfo& cur);
  int _MakeCgiUnknownNetworkTask(t_server const& server_config,
                                 t_uriInfo uriBlocks);

 public:
  UnknownNetworkTask(AConnection* connection, t_server const& server_config,
                     int const& fd);
  ~UnknownNetworkTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);

 private:
  string _method;
  string _UnknownNetworkTask_uri;
  string _http_version;
  string _query;
  map<string, string> _headers;
  unsigned int _content_len;
  unsigned int _client_max_body_size;  // from config
  bool _chunked;
  string _body;
  int status;
  int _code_status;

 public:
  // void modifyUri();
  // void parseBody(string& body);
  void parseHeaders(string headers);
  // void parseUnknownNetworkTask(string& buf);
  void parseFirstLine(string& firstLine);
  void getChunked(string& str);
  void getSimple(string& str);
  void print();
  void reset();
  // void parse(char* buf, int nbytes, size_t i);

  void parse(std::stringstream& str);
};

#endif

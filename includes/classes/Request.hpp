#ifndef REQUEST_HPP
#define REQUEST_HPP

#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"
#define CRLF "\r\n"
#define strnpos std::string::npos 

#include "../main.hpp"

using namespace std;

enum States{
    START,
    HEADERS,
    BODY,
    END
};

class Connection;

class Request : public ATask{
 private:
  int _fd;
  int _parentFd;
  int _status;
  Connection* _connection;
  std::stringstream _responseHeader;
  std::stringstream _responseBody;
  std::stringstream _response;

 public:
  Request(void);
  Request(Connection* connection, int const& fd);
  Request(Connection* connection, int const& fd, int const& parentFd);
  ~Request();
  int getFd(void) const;
  int getRequest(t_server const& server_config);
  int sendResult(void);
  int makeResponseFromString(std::string str);

 private:
  Request(Request const& src);
  Request& operator=(Request const& rhs);
  int _RequestHandler(t_server const& server_config);
  int _MakeResponseBody(t_server const& server_config, t_uriInfo &cur);
  int _MakeResponseHeaders(t_uriInfo &cur);
  int _AssembleRespose(void);
  int _MakeAutoIndex(std::string const& show_path, std::string const& real_path);
  int _MakeCgiRequest(t_server const& server_config, t_uriInfo uriBlocks);
  int _MakeStdRequest(std::string uri);


  private:
    string  _method;
    string  _request_uri;
    string  _http_version;
    string  _query; 
    map<string, string> _headers;
    unsigned int     _content_len;
    unsigned int     _client_max_body_size; //from config
    bool    _chunked;
    string  _body;
    int     status;
    int     _code_status;

public:
    // void modifyUri();
    // void parseBody(string& body);
    void parseHeaders(string headers);
    // void parseRequest(string& buf);
    void parseFirstLine(string& firstLine);
    void getChunked(string& str);
    void getSimple(string& str);
    void print();
    void reset(); 
    void parse(char *buf, int nbytes, size_t i);
};

#endif

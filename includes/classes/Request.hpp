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

enum Status { NEW, READING, READY_TO_HANDLE, READY_TO_SEND, SENDING, DONE };

class RequestManager;

class Request {
 private:
  int _fd;
  int _parentFd;
  int _status;
  RequestManager* _rm;
  std::stringstream _responseHeader;
  std::stringstream _responseBody;
  std::stringstream _response;

 public:
  Request(void);
  Request(RequestManager* rm, int const& fd);
  Request(RequestManager* rm, int const& fd, int const& parentFd);
  ~Request();
  int getFd(void) const;
  int getStatus(void) const;
  void setStatus(int status);
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
    int     _content_len;
    int     _client_max_body_size; //from config
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
    // void getChunked(string& str);
    // void getSimple(string& str);
    void print();
    void reset(); 
    void parse(char *buf, int nbytes, size_t i);
};

#endif

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../main.hpp"

enum Status { NEW, READING, READY_TO_HANDLE, READY_TO_SEND, SENDING, DONE };

class Request {
 private:
  int _fd;
  int _status;
  t_requestHeader _header;
  std::vector<char> _body;
  std::stringstream _responseHeader;
  std::stringstream _responseBody;
  std::stringstream _response;

 public:
  Request(void);
  Request(int const& fd);
  ~Request();
  int getFd(void) const;
  int getStatus(void) const;
  void setStatus(int status);
  int getRequest(void);
  int sendResult(void);

 private:
  Request(Request const& src);
  Request& operator=(Request const& rhs);
  int _RequestHandler(void);
  int _MakeResponseBody(void);
  int _MakeResponseHeaders(void);
  int _AssembleRespose(void);
};

#endif

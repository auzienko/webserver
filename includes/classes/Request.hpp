/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 09:55:20 by zcris             #+#    #+#             */
/*   Updated: 2022/03/12 09:02:00 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

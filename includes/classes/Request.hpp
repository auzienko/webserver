/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 09:55:20 by zcris             #+#    #+#             */
/*   Updated: 2022/03/11 11:24:50 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../main.hpp"

enum Status { NEW, READY, DONE };

class Request {
 private:
  int _fd;
  int _status;

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
};

#endif

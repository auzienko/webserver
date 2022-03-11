/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_manager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 10:10:38 by zcris             #+#    #+#             */
/*   Updated: 2022/03/11 11:18:36 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_MANAGER_HPP
#define REQUEST_MANAGER_HPP

#include "../main.hpp"

class Request;

class Request_manager {
 private:
  std::map<int, Request*> _list;

 public:
  Request_manager();
  ~Request_manager(); //реализовать, там зачистка с указателей

  void add(int fd);
  void remove(int fd);
  Request* at(int fd) const;
  int getRequest(int fd);
  int sendResult(int fd);
  
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_manager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 10:13:52 by zcris             #+#    #+#             */
/*   Updated: 2022/03/11 11:29:55 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Request_manager.hpp"

Request_manager::Request_manager() {}

Request_manager::~Request_manager() {
  //тут утечка
}

void Request_manager::add(int fd) {
  Request* tmp = new Request(fd);
  _list.insert(std::pair<int, Request*>(fd, tmp));
}

void Request_manager::remove(int fd) {
  std::map<int, Request*>::iterator it;
  it = _list.find(fd);
  if (it != _list.end()) {
    delete (*it).second;
    _list.erase(it);
  }
}

Request* Request_manager::at(int fd) const {
  std::map<int, Request*>::const_iterator it;
  it = _list.find(fd);
  if (it != _list.end()) {
    return (*it).second;
  }
  return nullptr;
}

int Request_manager::getRequest(int fd) {
  Request* tmp = at(fd);
  if (tmp != nullptr) return tmp->getRequest();
  return 0;
}

int Request_manager::sendResult(int fd) {
  Request* tmp = at(fd);
  if (tmp != nullptr && tmp->getStatus() == READY) return tmp->sendResult();
  return 0;
}

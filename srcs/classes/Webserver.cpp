/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:45:07 by zcris             #+#    #+#             */
/*   Updated: 2022/03/11 12:18:07 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Webserver.hpp"

Webserver::Webserver(int maxConnection)
    : _connectionCount(0), _maxConnection(maxConnection), _fd_max(-1) {
  FD_ZERO(&_connections);
  _rm = new Request_manager();
}

Webserver::~Webserver() { delete _rm; }

int const& Webserver::getClientsCount(void) const { return _connectionCount; }

void Webserver::plusConnection(void) {
  if (_connectionCount < _maxConnection) ++_connectionCount;
}

void Webserver::minusConnection(void) {
  if (_connectionCount != 0) --_connectionCount;
}

void Webserver::addConnection(int fd) {
  FD_SET(fd, &_connections);
  plusConnection();
  if (fd > _fd_max) _fd_max = fd;
  _rm->add(fd);
}

void Webserver::closeConnection(int fd) {
  if (FD_ISSET(fd, &_connections)) {
    close(fd);
    FD_CLR(fd, &_connections);
    minusConnection();
    if (fd == _fd_max) {
      while (FD_ISSET(_fd_max, &_connections) == false) _fd_max -= 1;
    }
  }
  _rm->remove(fd);
}

int Webserver::createServerListenSocket(void) {
  int err;
  struct sockaddr_in addr;

  _listenSocket = socket(PF_INET, SOCK_STREAM, 0);
  if (_listenSocket < 0) {
    ws::printE(ERROR_CREATE_SOCKET, "\n");
    return -1;
  }

  int const opt = 1;
  err = setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
                   sizeof(opt));
  if (err < 0) {
    ws::printE(ERROR_SOCKET_OPTIONS, "\n");
    close(_listenSocket);
    return -1;
  }
  
  addr.sin_family = AF_INET;
  //берем из конфига
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  //берем из конфига
  addr.sin_port = htons(DEFAULT_SERVER_PORT);
  
  err = bind(_listenSocket, (struct sockaddr*)&addr, sizeof(addr));
  if (err < 0) {
    ws::printE(ERROR_BIND_SOCKET, "\n");
    close(_listenSocket);
    return -1;
  }

  err = listen(_listenSocket, DEFAULT_LISTEN_QUEUE);
  if (err < 0) {
    ws::printE(ERROR_LISTEN_SOCKET, "\n");
    close(_listenSocket);
    return -1;
  }

  addConnection(_listenSocket);
  _fd_max = _listenSocket;
  return 0;
}

void Webserver::_ReadHandler(int fd) {
  if (fd == _listenSocket) {
    struct sockaddr_in client;
    socklen_t size = sizeof(client);
    int new_sock = accept(_listenSocket, (struct sockaddr*)&client, &size);
    if (new_sock < 0) {
      ws::printE(ERROR_ACCEPT, "\n");
      //добавить обработчик
      exit(EXIT_FAILURE);
    }
    if (_connectionCount < _maxConnection) {
      fcntl(new_sock, F_SETFL, O_NONBLOCK);
      addConnection(new_sock);
    } else {
      ws::printE(ERROR_SOCKET_COUNT_LIMIT, "\n");
      close(new_sock);
    }
  } else {
    if (_rm->getRequest(fd) < 0) {
      ws::printE(ERROR_READ_FROM_CLIENT, "\n");
      closeConnection(fd);
    }
  }
}

void Webserver::_WriteHandler(int fd) {
  if (fd == _listenSocket) return;
  if (_rm->at(fd) == nullptr || _rm->at(fd)->getStatus() != READY) return;
  ws::print(MESSAGE_TRY_SEND_DATA, " ");
  if (_rm->sendResult(fd) < 0) {
    ws::print(MESSAGE_FAIL, "\n");
    ws::printE(ERROR_SEND_TO_CLIENT, "\n");
    closeConnection(fd);
  } else {
    ws::print(MESSAGE_SUCCESS, "\n");
  }
}

int Webserver::run(void) {
  fd_set read_fds;
  fd_set write_fds;

  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);

  ws::print(MESSAGE_TRY_START_SERV, "\n");
  ws::print(MESSAGE_CREATE_LISTEN_SOCKET, " ");
  if (createServerListenSocket() != 0) {
    ws::print(MESSAGE_FAIL, "\n");
    exit(EXIT_FAILURE);
  }
  ws::print(MESSAGE_SUCCESS, "\n");
  ws::print(MESSAGE_SERVER_STARTED, "\n");

  // timeout
  struct timeval tv;
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  while (1) {
    read_fds = _connections;
    write_fds = _connections;
    if (select(_fd_max + 1, &read_fds, &write_fds, 0, &tv) < 0) {
      ws::printE(ERROR_SELECT, "\n");
      //добавить обработчик
      exit(EXIT_FAILURE);
    }
    int max_fd = _fd_max;
    for (int i = 0; i <= max_fd; ++i) {
      if (FD_ISSET(i, &read_fds)) _ReadHandler(i);
      if (FD_ISSET(i, &write_fds)) _WriteHandler(i);
    }
  }
  return 0;
}

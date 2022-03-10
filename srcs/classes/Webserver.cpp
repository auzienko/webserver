/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:45:07 by zcris             #+#    #+#             */
/*   Updated: 2022/03/10 13:08:34 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Webserver.hpp"

Webserver::Webserver(int maxConnection)
    : _connectionCount(0), _maxConnection(maxConnection), _fd_max(-1) {
  FD_ZERO(&_connections);
}

Webserver::~Webserver() {}

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

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = PF_INET;
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

void Webserver::closeConnection(int index) {
  if (FD_ISSET(index, &_connections)) {
    close(index);
    FD_CLR(index, &_connections);
    minusConnection();
  }
}

int Webserver::run(void) {
  fd_set read_fds;
  // fd_set write_fds;

  ws::print(MESSAGE_TRY_START_SERV, "\n");
  ws::print(MESSAGE_CREATE_LISTEN_SOCKET, " ");
  if (createServerListenSocket() != 0) {
    ws::print(MESSAGE_FAIL, "\n");
    exit(EXIT_FAILURE);
  }
  ws::print(MESSAGE_SUCCESS, "\n");
  ws::print(MESSAGE_SERVER_STARTED, "\n");

  // слушаем известные сокеты на предмет изменений в вечном цикле
  struct timeval tv;
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  while (1) {
    //FD_ZERO(&read_fds);
    FD_COPY(&_connections, &read_fds);
    //read_fds = _connections;
    // if (select(_listenSocket + 1, &read_fds, NULL, NULL, &tv) < 0) {
    if (select(_listenSocket + 1, &read_fds, NULL, NULL, NULL) < 0) {
      ws::printE(ERROR_SELECT, "\n");
      //добавить обработчик
      exit(EXIT_FAILURE);
    }
//    std::cout << "hello\n";
    for (int i = 0; i <= _fd_max; ++i) {
      if (FD_ISSET(i, &read_fds)) {
        if (i == _listenSocket) {
          struct sockaddr_in client;
          socklen_t size = sizeof(client);
          int new_sock =
              accept(_listenSocket, (struct sockaddr*)&client, &size);
          if (new_sock < 0) {
            ws::printE(ERROR_ACCEPT, "\n");
            //добавить обработчик
            exit(EXIT_FAILURE);
          }
          if (_connectionCount < _maxConnection) {
            fcntl(new_sock, F_SETFL, O_NONBLOCK);
            addConnection(new_sock);
            //break ;
            FD_SET(new_sock, &read_fds);
          } else {
            ws::printE(ERROR_SOCKET_COUNT_LIMIT, "\n");
            close(new_sock);
          }
        } else {
          if (Request_manager::getRequest(i) < 0) {
            //ошибка или конец данных
            ws::printE(ERROR_READ_FROM_CLIENT, "\n");
            closeConnection(i);
            FD_CLR(i, &read_fds);
            --i;
          } else {
            ws::print(MESSAGE_TRY_SEND_DATA, " ");
            if (Request_manager::sendResult(i) < 0) {
              ws::print(MESSAGE_FAIL, "\n");
              ws::printE(ERROR_SEND_TO_CLIENT, "\n");
              closeConnection(i);
              FD_CLR(i, &read_fds);
              --i;
            } else {
              ws::print(MESSAGE_SUCCESS, "\n");
            }
          }
        }
      }
    }
  }
  return 0;
}

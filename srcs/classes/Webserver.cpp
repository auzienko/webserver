/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:45:07 by zcris             #+#    #+#             */
/*   Updated: 2022/02/07 18:19:57 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Webserver.hpp"

Webserver::Webserver(Config* config, int maxConnection)
    : _config(config), _connectionCount(0), _maxConnection(maxConnection) {}

Webserver::~Webserver() {}

void Webserver::banner(void) const {
  ws::print("==================================", "\n");
  ws::print(PROGRAMM_NAME, " ");
  ws::print(PROGRAMM_VERSION, "\n");
  ws::print(AUTHORS, "\n");
  ws::print("==================================", "\n");
}

int const& Webserver::getClientsCount(void) const { return _connectionCount; }

void Webserver::plusConnection(void) {
  if (_connectionCount < _maxConnection) ++_connectionCount;
}

void Webserver::minusConnection(void) {
  if (_connectionCount != 0) --_connectionCount;
}

void Webserver::addConnection(int fd, short events) {
  pollfd tmp;

  tmp.fd = fd;
  tmp.events = events;
  tmp.revents = 0;
  _connections.push_back(tmp);
  plusConnection();
}

int Webserver::createServerListenSockets(void) {
  int sock, err;
  struct sockaddr_in addr;
  pollfd tmp;

  //сдеалать в цикле при разборе конфига
  //добавить проверку, чтобы слушашие соединения не превысили количество
  //коннектинов максимальное
  for (int i = 0; i < 1; ++i) {
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      ws::printE(ERROR_CREATE_SOCKET, "\n");
      return -1;
    }
    int const opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(DEFAULT_SERVER_PORT);
    err = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (err < 0) {
      ws::printE(ERROR_BIND_SOCKET, "\n");
      close(sock);
      return -1;
    }
    err = listen(sock, DEFAULT_LISTEN_QUEUE);
    if (err < 0) {
      ws::printE(ERROR_LISTEN_SOCKET, "\n");
      close(sock);
      return -1;
    }
    tmp.fd = sock;
    tmp.events = POLLIN;
    tmp.revents = 0;
    _connections.push_back(tmp);
    _listenSockets.push_back(sock);
    plusConnection();
  }
  return 0;
}

void Webserver::closeConnection(int index) {
  close(_connections[index].fd);
  _connections.erase(_connections.begin() + index);
  minusConnection();
}

int Webserver::sendResult(int fd, char* buf) {
  int nbytes, ret;
  std::stringstream http;
  std::stringstream html;

  char* p = strstr(buf, "index.html");

  if (p && p - buf < 20) {
    html << "<!DOCTYPE html>\r\n";
    html << "<html>\r\n";
    html << "<head>\r\n";
    html << "<meta charset=\"UTF-8\">\r\n";
    html << "<title>Title test</title>\r\n";
    html << "</head>\r\n";
    html << "<body>\r\n";
    html << "<h2> TEST </h2> \r\n";
    html << "<p>poll — это более новый метод опроса сокетов, созданный после "
            "того, как люди начали пытаться писать большие и высоконагруженные "
            "сетевые сервисы. Он спроектирован намного лучше и не страдает от "
            "большинства недостатков метода select. В большинстве случаев при "
            "написании современных приложений вы будете выбирать между "
            "использованием poll и epoll/libevent.</p>\r\n";
    html << "</body>\r\n";
    html << "</html>\r\n";

    http << "HTTP/1.1 200 OK\r\n";
    http << "Connection: keep-alive\r\n";
    http << "Content-type: text/html\r\n";
    http << "Content-lenght: " << html.str().length() << "\r\n";
    http << "\r\n";
    http << html.str();
    ret = 0;
  } else {
    http << "HTTP/1.1 404 Not Found\r\n";
    http << "Connection: close\r\n";
    http << "Content-lenght: 0\r\n";
    http << "Content-Type: text/html\r\n";
    http << "\r\n";
    ret = -1;
  }

  nbytes = send(fd, http.str().c_str(), http.str().length() + 1, 0);
  if (nbytes < 0) ret = -1;

  printf("Server: write return %d\n", ret);

  return ret;
}

int Webserver::getRequest(int fd, char* buf) {
  int nbytes;

  nbytes = recv(fd, buf, DEFAULT_BUFLEN, 0);
  fprintf(stdout, "reading %d bytes from socket %d\n", nbytes, fd);
  if (nbytes < 0) {
    //ошибка чтения
    perror("Server: read failture");
    return -1;
  } else if (nbytes == 0) {
    fprintf(stdout, "reading no data\n");
    return 0;
  } else {
    int cnt = 0;
    printf("\n\nServer gets %d bytes:\n", nbytes);

    for (int i = 0; i < nbytes && cnt < 2; ++i) {
      write(1, &buf[i], 1);
    }
    write(1, "\n\n", 2);
    return 0;
  }
}

int Webserver::run(void) {
  char buf[DEFAULT_BUFLEN];

  ws::print(MESSAGE_TRY_START_SERV, "\n");
  // create _listenSockets (серверные сокеты из конфига)
  // подумать как корректно выходить.
  ws::print(MESSAGE_CREATE_LISTEN_SOCKETS, " ");
  if (createServerListenSockets() != 0) {
    ws::print(MESSAGE_FAIL, "\n");
    exit(EXIT_FAILURE);
  }
  ws::print(MESSAGE_SUCCESS, "\n");
  ws::print(MESSAGE_SERVER_STARTED, "\n");

  // слушаем известные сокеты на предмет изменений в вечном цикле
  while (1) {
    int ret = poll((pollfd*)&_connections[0], _connectionCount, -1);
    //-1 задержка. если -1 то происходит блокировка на этой функции.
    if (ret < 0) {
      ws::printE(ERROR_POLL, "\n");
      //добавить обработчик
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < _connectionCount; ++i) {
      if (_connections[i].revents & POLLIN) {
        _connections[i].revents &= ~POLLIN;
        //добавить функцию выбора листен соединений а не только 0
        if (i == 0) {
          struct sockaddr_in client;
          socklen_t size = sizeof(client);
          int new_sock =
              accept(_connections[i].fd, (struct sockaddr*)&client, &size);
          if (new_sock < 0) {
            ws::printE(ERROR_ACCEPT, "\n");
            //добавить обработчик
            exit(EXIT_FAILURE);
          }
          if (_connectionCount < _maxConnection)
            addConnection(new_sock, POLLIN);
          else {
            ws::printE(ERROR_SOCKET_COUNT_LIMIT, "\n");
            close(new_sock);
          }
        } else {
          if (getRequest(_connections[i].fd, buf) < 0) {
            //ошибка или конец данных
            ws::printE(ERROR_READ_FROM_CLIENT, "\n");
            closeConnection(i);
            --i;
          } else {
            ws::print(MESSAGE_TRY_SEND_DATA, " ");
            if (sendResult(_connections[i].fd, buf) < 0) {
              ws::print(MESSAGE_FAIL, "\n");
              ws::printE(ERROR_SEND_TO_CLIENT, "\n");
              closeConnection(i);
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

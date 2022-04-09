#include "../../includes/classes/Webserver.hpp"

Webserver::Webserver(t_server &src, int maxConnection)
    : _maxConnection(maxConnection), _serverConfig(src) {
  _connectionManager = new ConnectionManager();
}

Webserver::~Webserver() { delete _connectionManager; }

int Webserver::getClientsCount(void) const { return _connectionManager->getConnectionCount(); }

void Webserver::addConnection(int fd) {
  _connectionManager->add(new NetworkConnection(_connectionManager, fd));
}

void Webserver::closeConnection(int fd) {
  if (_connectionManager->isExist(fd)) {
    _connectionManager->remove(fd);
  }
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
  addr.sin_port = htons(_serverConfig.port);
  if (inet_pton(addr.sin_family, _serverConfig.listen.c_str(),
                &(addr.sin_addr)) < 0) {
    ws::printE(ERROR_CREATE_IP, "\n");
    close(_listenSocket);
    return -1;
  }
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
  //addConnection(_listenSocket);

  char str[sizeof(addr)];
  inet_ntop(AF_INET, &(addr.sin_addr), str, sizeof(addr));
  std::cout << "webserver: " << str << ":" << ntohs(addr.sin_port) << " ";

  return 0;
}

int Webserver::readHandler(int fd) {
  if (fd != _listenSocket && _connectionManager->isExist(fd) == 0)
    return -1;
  if (fd == _listenSocket) {
  
    struct sockaddr_in client;
    socklen_t size = sizeof(client);
    int new_sock = accept(_listenSocket, (struct sockaddr*)&client, &size);
    if (new_sock < 0) {
      ws::printE(ERROR_ACCEPT, "\n");
      //добавить обработчик
      exit(EXIT_FAILURE);
    }
    if (_connectionManager->getConnectionCount() < _maxConnection) {
      fcntl(new_sock, F_SETFL, O_NONBLOCK);
      addConnection(new_sock);
    } else {
      ws::printE(ERROR_SOCKET_COUNT_LIMIT, "\n");
      close(new_sock);
    }
  } else {
    if (_connectionManager->readData(fd, _serverConfig) < 0) {
      ws::printE(ERROR_READ_FROM_CLIENT, "\n");
      closeConnection(fd);
    }
  }
  return 0;
}

int Webserver::writeHandler(int fd) {
  if (fd == _listenSocket) return 0;
  if (_connectionManager->isExist(fd) == 0) return -1;
  if (_connectionManager->at(fd) == nullptr ||
      _connectionManager->at(fd)->getTask() == nullptr ||
      _connectionManager->at(fd)->getTask()->getStatus() != SENDING)
    return 0;
  ws::print(MESSAGE_TRY_SEND_DATA, " ");
  if (_connectionManager->sendData(fd) < 0) {
    ws::print(MESSAGE_FAIL, "\n");
    ws::printE(ERROR_SEND_TO_CLIENT, "\n");
  } else {
    ws::print(MESSAGE_SUCCESS, "\n");
  }
  //понять когда закрывать коннекшены корректно
  //closeConnection(fd);
  return 0;
}

int Webserver::run(void) {
  ws::print(MESSAGE_TRY_START_SERV, "\n");
  ws::print(MESSAGE_CREATE_LISTEN_SOCKET, " ");
  if (createServerListenSocket() != 0) {
    ws::print(MESSAGE_FAIL, "\n");
    exit(EXIT_FAILURE);
  }
  ws::print(MESSAGE_SUCCESS, "\n");
  ws::print(MESSAGE_SERVER_STARTED, "\n");
  return 0;
}

void Webserver::makeActiveFdsSet(fd_set* fds, int* max_fd) const {
  FD_SET(_listenSocket, fds);
  *max_fd = _listenSocket > *max_fd ? _listenSocket : *max_fd;
  std::vector<int> tmp = _connectionManager->getAllConnectionsFds();
  std::vector<int>::const_iterator i = tmp.begin();
  std::vector<int>::const_iterator e = tmp.end();
  while (i != e) {
    FD_SET(*i, fds);
    *max_fd = *i > *max_fd ? *i : *max_fd;
    ++i;
  }
}

void Webserver::closeConnectionIfTimout(int seconds){
  _connectionManager->closeConnectionIfTimout(seconds);
}

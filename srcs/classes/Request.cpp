/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 10:03:34 by zcris             #+#    #+#             */
/*   Updated: 2022/03/11 11:48:34 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Request.hpp"

Request::Request(void) : _fd(-1), _status(NEW) {}

Request::~Request(void) {}

Request::Request(int const& fd) : _fd(fd), _status(NEW) {}

int Request::getFd(void) const { return _fd; }

int Request::getStatus(void) const { return _status; }

void Request::setStatus(int status) { _status = status; }

int Request::sendResult(void) {
  int nbytes, ret;
  int fd = _fd;
//   char buf[DEFAULT_BUFLEN];
  std::stringstream http;
  std::stringstream html;

//   char* p = strstr(buf, "index.html");

//   if (p && p - buf < 20) {
	if(1){
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

  printf("Server: write return %d ", ret);

  setStatus(DONE);

  return ret;
}

int Request::getRequest(void) {
  int nbytes;
  char buf[DEFAULT_BUFLEN];
  int fd = _fd;
  // https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
  // https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html
  //необходимо сделать чтение пока не встретим пустую строку (типа гетнекст
  //лайна) далее необходимо сделать разбор заголовка с сложить в структуру
  //t_requestHeader т.е читаем из входящего fd пока не встретим пустую строку,
  //думаю можно побайтово пока не встретим комбинацию \r\n\r\n все что читали
  //куда-то сохраняли то что насохраняли отдаем в парсер, который собирает
  //структуру.

  // GET /hello.htm HTTP/1.1
  // User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
  // Host: www.tutorialspoint.com
  // Accept-Language: en-us
  // Accept-Encoding: gzip, deflate
  // Connection: Keep-Alive

  // POST /cgi-bin/process.cgi HTTP/1.1
  // User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
  // Host: www.tutorialspoint.com
  // Content-Type: application/x-www-form-urlencoded
  // Content-Length: length
  // Accept-Language: en-us
  // Accept-Encoding: gzip, deflate
  // Connection: Keep-Alive
  // r\n\r\n
  // licenseID=string&content=string&/paramsXML=string

  // POST /cgi-bin/process.cgi HTTP/1.1
  // User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
  // Host: www.tutorialspoint.com
  // Content-Type: text/xml; charset=utf-8
  // Content-Length: length
  // Accept-Language: en-us
  // Accept-Encoding: gzip, deflate
  // Connection: Keep-Alive
  // пустая строка
  // <?xml version="1.0" encoding="utf-8"?>
  // <string xmlns="http://clearforest.com/">string</string>

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


	setStatus(READY);
	
    return 0;
  }
}

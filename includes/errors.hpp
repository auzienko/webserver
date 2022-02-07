#ifndef ERRORS_HPP
#define ERRORS_HPP

#define ERROR_PROGRAM_ARGS \
  "Incorrect argument count.\nusage: ./webserver congig_file_name.21ws"
#define ERROR_CREATE_SOCKET "Socket was not created"
#define ERROR_BIND_SOCKET "Socket was not binded"
#define ERROR_LISTEN_SOCKET "Listen queue failure"
#define ERROR_POLL "Poll failure"
#define ERROR_ACCEPT "Accept failure"
#define ERROR_SOCKET_COUNT_LIMIT "No more sockets for client"
#define ERROR_SEND_TO_CLIENT "Can't send data to client"
#define ERROR_READ_FROM_CLIENT "Can't read from client"
#endif

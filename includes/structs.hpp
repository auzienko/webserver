#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include "main.hpp"

typedef struct s_redir {
  int code;
  std::string target;

  s_redir(void) : code(0) {}
} t_redir;

typedef struct tag_location {
  std::string path;
  std::string root;
  std::set<std::string> methods;
  std::string index;
  size_t client_max_body_size;
  std::string uploads_path;
  t_redir redir;
  bool autoindex;
  bool file_uploads;

  tag_location(void)
      : client_max_body_size(0), autoindex(false), file_uploads(false) {}
} t_location;

typedef struct tag_server {
  std::string listen;
  size_t port;
  std::string server_name;
  size_t client_max_body_size;
  std::vector<t_location> locations;
  std::map<int, std::string> error_pages;
  std::map<std::string, std::string>
      cgi;  //расширение - ключ, значение - путь к cgi-файлу
  std::map<std::string, std::set<std::string> > cgi_methods;
} t_server;

typedef struct s_uriInfo{
    std::string uri;
    std::string upload;
    bool isCgi;
    std::set<std::string> cgi_methods;
    std::string args;
    std::string pathInfo;
    const t_location *loc;

    s_uriInfo(void) : isCgi(false), loc(0) {}
}   t_uriInfo;

#endif

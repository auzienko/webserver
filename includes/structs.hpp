#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include "main.hpp"

typedef struct tag_location {
  std::string path;
  std::string root;
  std::set<std::string> methods;
  std::string index;
  size_t client_max_body_size;
  std::string uploads_path;
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
} t_server;

typedef struct s_uriInfo{
    std::string uri;
    bool        isCgi;
    std::string args;
    std::string pathInfo;
    const t_location  *loc;
}   t_uriInfo;

#endif

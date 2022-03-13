#include "main.hpp"


typedef struct tag_methods {
  bool  get;
  bool  post;
  bool  del;

  tag_methods(void) : get(false), post(false), del(false) {}
} t_methods;

typedef struct tag_location {
  std::string path;
  std::string root;
  t_methods   methods;
  std::string index;
  size_t      client_max_body_size;
  std::string uploads_path;
  bool        autoindex;
  bool        file_uploads;

  tag_location(void) : client_max_body_size(0), autoindex(false), file_uploads(false) {}
} t_location;

typedef struct tag_server {
  std::string                         listen;
  size_t                              port;
  std::string                         server_name;
  size_t                              client_max_body_size;
  std::vector<t_location>             locations;
  std::map<int, std::string>          error_pages;
  std::map<std::string, std::string>  cgi;                  //расширение - ключ, значение - путь к cgi-файлу
} t_server;

typedef struct tag_requestHeader {
    std::string Method;
    std::string Request_URI;
    std::string HTTP_Version;
    std::string Host;
    
    std::string Connection;
    int Content_Length;

    std::string Cache_Control;
    std::string Accept_Encoding;
    std::string Accept_Language;
    std::string User_Agent;
    int Upgrade_Insecure_Requests;
    
    std::string Accept;
    std::string Accept_Charset;
    std::string Authorization;
    std::string Expect;
    std::string From;
    std::string If_Match;
    std::string If_Modified_Since;
    std::string If_None_Match;
    std::string If_Range;
    std::string If_Unmodified_Since;
    std::string Max_Forwards;
    std::string Proxy_Authorization;
    std::string Range;
    std::string Referer;
    std::string TE;
} t_requestHeader;

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:29:26 by zcris             #+#    #+#             */
/*   Updated: 2022/02/08 12:02:45 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

typedef struct tag_location {
  std::string path;
  std::string root;
  std::vector<std::string> methods;
  std::vector<std::string> index;
  size_t client_max_body_size;
  std::string mime_conf_path;
  std::string redirect;
  std::string cgi_path;
  bool autoindex;
  bool file_uploads;
} t_location;

typedef struct tag_server {
  std::string listen;
  size_t port;
  std::string server_name;
  size_t client_max_body_size;
  std::string mime_conf_path;
  std::vector<t_location> locations;
} t_server;

typedef struct tag_requestHeader {
    std::string Method;
    std::string Request-URI;
    std::string HTTP-Version;
    std::string Host;
    std::string Connection;
    int Content-Length;

    std::string Cache-Control;
    std::string Accept-Encoding;
    std::string Accept-Language;
    std::string User-Agent;
    int Upgrade-Insecure-Requests;
    
    std::string Accept;
    std::string Accept-Charset;
    std::string Authorization;
    std::string Expect;
    std::string From;
    std::string If-Match;
    std::string If-Modified-Since;
    std::string If-None-Match;
    std::string If-Range;
    std::string If-Unmodified-Since;
    std::string Max-Forwards;
    std::string Proxy-Authorization;
    std::string Range;
    std::string Referer;
    std::string TE;
} t_requestHeader;
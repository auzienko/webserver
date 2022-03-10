/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wgaunt <wgaunt@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:29:26 by zcris             #+#    #+#             */
/*   Updated: 2022/03/10 16:35:36 by wgaunt           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"


typedef struct tag_methods {
  bool  get;
  bool  post;
  bool  del;
} t_methods;

typedef struct tag_location {
  std::string path;
  std::string root;
  t_methods   methods;
  std::string index;
  size_t      client_max_body_size;
  std::string mime_conf_path;
  std::string redirect;
  std::string cgi_path;
  bool        autoindex;
  bool        file_uploads;
} t_location;

typedef struct tag_server {
  std::string                 listen;
  size_t                      port;
  std::string                 server_name;
  size_t                      client_max_body_size;
  std::string                 mime_conf_path;
  std::vector<t_location>     locations;
  std::map<int, std::string>  error_pages;
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

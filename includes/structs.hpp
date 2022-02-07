/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:29:26 by zcris             #+#    #+#             */
/*   Updated: 2022/02/07 12:09:17 by zcris            ###   ########.fr       */
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

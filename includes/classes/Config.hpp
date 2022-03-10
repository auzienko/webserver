/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wgaunt <wgaunt@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 10:47:43 by zcris             #+#    #+#             */
/*   Updated: 2022/03/10 20:13:37 by wgaunt           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define WHITE_SPACES "\n\t "
#include "../main.hpp"

class Config {
 private:
  std::string   _config_file;
  std::ifstream _file_stream;
  t_server      _server;

  Config(void);
  Config(Config const& src);
  Config& operator=(Config const& rhs);

  void  _setDefaultServer(void);

 public:
  Config(std::string const& file_name);
  ~Config(void);

  int       checkAndParse(void);
  t_server  &get_server(void);
};

#endif

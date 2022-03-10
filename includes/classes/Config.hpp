/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 10:47:43 by zcris             #+#    #+#             */
/*   Updated: 2022/02/07 12:11:35 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../main.hpp"

class Config {
 private:
  std::string _config_file;
  std::vector<t_server> _servers;

  Config(void);
  Config(Config const& src);
  Config& operator=(Config const& rhs);

 public:
  Config(std::string const& file_name);
  int checkAndParse(void);
  ~Config(void);
};

#endif

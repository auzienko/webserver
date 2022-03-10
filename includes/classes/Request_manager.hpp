/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_manager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 10:10:38 by zcris             #+#    #+#             */
/*   Updated: 2022/03/10 10:19:01 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_MANAGER_HPP
#define REQUEST_MANAGER_HPP

#include "../main.hpp"

class Request_manager {
 public:
  Request_manager();
  ~Request_manager();

  static int sendResult(int fd);
  static int getRequest(int fd);
};

#endif

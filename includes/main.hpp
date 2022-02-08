/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 10:09:50 by zcris             #+#    #+#             */
/*   Updated: 2022/02/08 10:41:56 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>


#include "default_settings.hpp"
#include "errors.hpp"
#include "messages.hpp"
#include "http_status_codes.hpp"
#include "structs.hpp"
#include "./classes/Config.hpp"
#include "./classes/Webserver.hpp"

#define PROGRAMM_NAME "21_WEBSERVER"
#define PROGRAMM_VERSION "0.01"
#define AUTHORS "vlucilla, wgaunt, zcris"

namespace ws {
void printE(std::string const& msg, std::string const& divider);
void print(std::string const& msg, std::string const& divider);
}  // namespace ws

#endif

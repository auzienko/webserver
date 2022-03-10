/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 10:55:56 by zcris             #+#    #+#             */
/*   Updated: 2022/02/07 11:11:47 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/classes/Config.hpp"

Config::Config(std::string const& file_name) : _config_file(file_name) {}

Config::~Config(void) {}

int Config::checkAndParse(void) { return 0; }

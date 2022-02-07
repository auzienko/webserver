/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 10:29:18 by zcris             #+#    #+#             */
/*   Updated: 2022/02/07 12:11:01 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.hpp"

void ws::printE(std::string const& msg, std::string const& divider) {
  std::cerr << msg << divider;
}

void ws::print(std::string const& msg, std::string const& divider) {
  std::cout << msg << divider;
}

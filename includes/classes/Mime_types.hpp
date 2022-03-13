/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime_types.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 11:26:08 by zcris             #+#    #+#             */
/*   Updated: 2022/03/12 12:01:31 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_TYPES_HPP
#define MIME_TYPES_HPP

#define MIMETYPESCOUNT 347

#include <iostream>

class Mime_types {
 private:
  struct pair {
    std::string fileExtension;
    std::string mimeType;
  };
  static Mime_types::pair _types[MIMETYPESCOUNT];
  Mime_types(Mime_types const& src);
  Mime_types& operator=(Mime_types const& rhs);

 public:
  Mime_types(void);
  ~Mime_types(void);
  static std::string getMimeType(std::string const& ext);
};

#endif

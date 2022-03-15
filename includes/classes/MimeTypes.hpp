#ifndef MIMETYPES_HPP
#define MIMETYPES_HPP

#define MIMETYPESCOUNT 347

#include <iostream>

class MimeTypes {
 private:
  struct pair {
    std::string fileExtension;
    std::string mimeType;
  };
  static MimeTypes::pair _types[MIMETYPESCOUNT];
  MimeTypes(MimeTypes const& src);
  MimeTypes& operator=(MimeTypes const& rhs);

 public:
  MimeTypes(void);
  ~MimeTypes(void);
  static std::string getMimeType(std::string const& ext);
};

#endif

#ifndef MAIN_HPP
#define MAIN_HPP

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>

#include <cstring>

#include "default_settings.hpp"
#include "errors.hpp"
#include "messages.hpp"
#include "z_array.hpp"
#include "http_status_codes.hpp"
#include "structs.hpp"
// #include "./classes/ATask.hpp"
#include "./classes/Config.hpp"
// #include "./classes/Webserver.hpp"
// #include "./classes/WebserverManager.hpp"
// #include "./classes/Request.hpp"
// #include "./classes/AConnection.hpp"
// #include "./classes/ConnectionManager.hpp"
// #include "./classes/MimeTypes.hpp"
// #include "./classes/ConfigUtils.hpp"

// #include "./classes/LocalConnection.hpp"
// #include "./classes/NetworkConnection.hpp"

#define PROGRAMM_NAME "21_WEBSERVER"
#define PROGRAMM_VERSION "0.01"
#define AUTHORS "vlucilla, wgaunt, zcris"

namespace ws {
void printE(std::string const& msg, std::string const& divider);
void print(std::string const& msg, std::string const& divider);
std::pair<bool, std::vector<std::string> > filesReadDirectory(std::string const& path);
bool filesIsDir(std::string const &file);
std::string intToStr(int i);
std::string stringTail(std::string const& str, char c);
std::string stringReverse(std::string& str);
std::string stringUrlDecode(std::string const& str);
std::vector<std::string> stringTrim(std::string line);
std::string socketGetIP(int const& fd);
std::string stringFromMap(std::map<std::string, std::string>::const_iterator it,
    std::map<std::string, std::string>::const_iterator end);
std::string stringToCGIFormat(std::string const& str);
bool uriIsCGI(std::vector<std::string> dirs, std::map<std::string, std::string> const& cgi, t_uriInfo &res);
void uriFill(std::string const &line, t_server const& server_config, t_uriInfo &res);
void stringSkipWS(std::string &line);
}  // namespace ws

#endif

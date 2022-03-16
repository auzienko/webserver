#ifndef MAIN_HPP
#define MAIN_HPP

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

#include "default_settings.hpp"
#include "errors.hpp"
#include "messages.hpp"
#include "http_status_codes.hpp"
#include "structs.hpp"
#include "./classes/Config.hpp"
#include "./classes/Webserver.hpp"
#include "./classes/WebserverManager.hpp"
#include "./classes/Request.hpp"
#include "./classes/RequestManager.hpp"
#include "./classes/MimeTypes.hpp"
#include "./classes/ConfigUtils.hpp"

#define PROGRAMM_NAME "21_WEBSERVER"
#define PROGRAMM_VERSION "0.01"
#define AUTHORS "vlucilla, wgaunt, zcris"

namespace ws {
void printE(std::string const& msg, std::string const& divider);
void print(std::string const& msg, std::string const& divider);
std::pair<bool, std::vector<std::string> > filesReadDirectory(std::string const& path);
}  // namespace ws

#endif

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
#include <fstream>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "default_settings.hpp"
#include "errors.hpp"
#include "messages.hpp"
#include "http_status_codes.hpp"
#include "structs.hpp"
#include "./classes/Config.hpp"
#include "./classes/Webserver.hpp"
#include "./classes/Webserver_manager.hpp"
#include "./classes/Request.hpp"
#include "./classes/Request_manager.hpp"
#include "./classes/Mime_types.hpp"

#define PROGRAMM_NAME "21_WEBSERVER"
#define PROGRAMM_VERSION "0.01"
#define AUTHORS "vlucilla, wgaunt, zcris"

namespace ws {
void printE(std::string const& msg, std::string const& divider);
void print(std::string const& msg, std::string const& divider);
}  // namespace ws

#endif

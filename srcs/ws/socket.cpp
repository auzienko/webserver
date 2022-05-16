#include "../../includes/main.hpp"

std::string ws::socketGetIP(int const& fd) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  char result[16];

  bzero(&addr, len);
  getsockname(fd, (struct sockaddr *)&addr, &len);
  inet_ntop(AF_INET, &addr.sin_addr, result, sizeof(result));
  return result;
}

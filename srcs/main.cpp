#include "../includes/main.hpp"
#include "../includes/classes/Webservers/WebserverManager.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    ws::printE(ERROR_PROGRAM_ARGS, "\n");
    return -1;
  }
  try {
    WebserverManager wsm(argv[1]);
    wsm.start();    
  } catch (const std::exception& e) {
    ws::printE(ERROR_SERVER_START, ":\n");
    ws::printE(e.what(), "\n");
    return -1;
  }
  return 0;
}

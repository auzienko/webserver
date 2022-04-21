#include "includes/main.hpp"

int main() {
	std::cout << "Content-Type: text/html\r\n\r\n";

	std::cout << "<html> <head>\r\n";
	std::cout << "<title>Hello,  World!</title>";
	std::cout << "</head>\r\n";
	std::cout << "<body>\r\n";
	std::cout << "<h1>Hello,  World!</h1>\r\n";
	std::cout << "</body> </html>\r\n"; 
	return 0;
}

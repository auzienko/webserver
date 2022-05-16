#include "../includes/main.hpp"

int main(int argc, char **argv, char **env) {
	std::string str("");

	std::cin >> str;

	std::cout << "Content-Type: text/html\r\n\r\n";

	std::cout << "<html> <head>\n";
	std::cout << "<title>Hello,  World!</title>";
	std::cout << "</head>\n";
	std::cout << "<body>\n";
	std::cout << "<h2>START</h2><br/>";
	std::cout << "<h1>" << str << "</h1>";
	for (int i = 0; env[i] != NULL; i++)
	{
		std::cout << env[i] << "<br/>" << std::endl;
	}
	std::cout << "<h2>END</h2><br/>";
	std::cout << "</body> </html>\n"; 
	return 0;
}

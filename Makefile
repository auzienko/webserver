NAME	= 	webserver

SRC_FOLDER = ./srcs/
HEADER_FOLDER = ./includes/

SRC_LIST = \
main.cpp \
ws/print.cpp \
ws/files.cpp \
ws/int.cpp \
ws/string.cpp \
ws/uri.cpp \
ws/socket.cpp \
classes/Tasks/ATask.cpp \
classes/Tasks/UnknownNetworkTask.cpp \
classes/Tasks/AutoindexTask.cpp \
classes/Tasks/GetTask.cpp \
classes/Tasks/RedirTask.cpp \
classes/Tasks/CgiParentTask.cpp \
classes/Tasks/CgiInputTask.cpp \
classes/Tasks/CgiOutputTask.cpp \
classes/Connections/AConnection.cpp \
classes/Connections/NetworkConnection.cpp \
classes/Connections/LocalConnection.cpp \
classes/Connections/ConnectionManager.cpp \
classes/Webservers/Webserver.cpp \
classes/Webservers/WebserverManager.cpp \
classes/Config.cpp \
classes/MimeTypes.cpp \
classes/HTTPCodes.cpp \
classes/ConfigUtils.cpp \
z_array/z_array_utils.cpp \
z_array/z_array.cpp

HEADER_LIST = \
main.hpp \
http_status_codes.hpp \
default_settings.hpp \
structs.hpp \
errors.hpp \
messages.hpp \
z_array.hpp \
classes/Tasks/ATask.hpp \
classes/Tasks/UnknownNetworkTask.hpp \
classes/Tasks/AutoindexTask.hpp \
classes/Tasks/GetTask.hpp \
classes/Tasks/RedirTask.hpp \
classes/Tasks/CgiParentTask.hpp \
classes/Tasks/CgiInputTask.hpp \
classes/Tasks/CgiOutputTask.hpp \
classes/Connections/AConnection.hpp \
classes/Connections/NetworkConnection.hpp \
classes/Connections/LocalConnection.hpp \
classes/Connections/ConnectionManager.hpp \
classes/Webservers/Webserver.hpp \
classes/Webservers/WebserverManager.hpp \
classes/Config.hpp \
classes/MimeTypes.hpp \
classes/HTTPCodes.hpp \
classes/ConfigUtils.hpp \

SRCS = $(addprefix $(SRC_FOLDER), $(SRC_LIST))
HEADERS = $(addprefix $(HEADER_FOLDER), $(HEADER_LIST))

OBJS	=	$(SRCS:.cpp=.o)
CC		=	c++ -g -fsanitize=address
CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic-errors
RM		=	rm -f
ALL_HEADERS = $(addprefix -I$(HEADER_FOLDER), $(HEADER_LIST))

OTHER_DEPEND = \
Makefile

%.o	:	%.cpp $(HEADERS)
	@$(CC) $(CFLAGS) $(ALL_HEADERS) -o $@ -c $<

$(NAME):	$(OTHER_DEPEND) $(OBJS) $(HEADERS) 
	$(CC) $(CFLAGS) $(ALL_HEADERS) $(OBJS) -o $(NAME)
	$(CC) cgi.cpp -o cgi/my_cgi.test #DONT FORGET TO RM IT!!!

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean $(NAME)

.PHONY:	all clean fclean re
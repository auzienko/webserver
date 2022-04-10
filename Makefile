NAME	= 	webserver

SRC_FOLDER = ./srcs/
HEADER_FOLDER = ./includes/

SRC_LIST = \
main.cpp \
ws/print.cpp \
ws/files.cpp \
ws/int.cpp \
ws/string.cpp \
ws/socket.cpp \
classes/Config.cpp \
classes/ATask.cpp \
classes/AConnection.cpp \
classes/NetworkConnection.cpp \
classes/Webserver.cpp \
classes/WebserverManager.cpp \
classes/ConnectionManager.cpp \
classes/Request.cpp \
classes/MimeTypes.cpp \
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
classes/Config.hpp \
classes/ATask.hpp \
classes/AConnection.hpp \
classes/NetworkConnection.hpp \
classes/Webserver.hpp \
classes/WebserverManager.hpp \
classes/ConnectionManager.hpp \
classes/Request.hpp \
classes/MimeTypes.hpp \
classes/ConfigUtils.hpp

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

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean $(NAME)

.PHONY:	all clean fclean re
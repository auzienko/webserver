NAME = webserver

SRCS =  srcs/main.cpp srcs/ws/print.cpp srcs/ws/files.cpp srcs/ws/int.cpp \
		srcs/ws/string.cpp srcs/ws/uri.cpp srcs/ws/socket.cpp \
		srcs/classes/Tasks/ATask.cpp srcs/classes/Tasks/UnknownNetworkTask.cpp \
		srcs/classes/Tasks/AutoindexTask.cpp srcs/classes/Tasks/GetTask.cpp \
		srcs/classes/Tasks/HeadTask.cpp srcs/classes/Tasks/RedirTask.cpp \
		srcs/classes/Tasks/CgiParentTask.cpp srcs/classes/Tasks/CgiInputTask.cpp \
		srcs/classes/Tasks/CgiOutputTask.cpp srcs/classes/Connections/AConnection.cpp \
		srcs/classes/Connections/NetworkConnection.cpp srcs/classes/Connections/LocalConnection.cpp \
		srcs/classes/Connections/ConnectionManager.cpp srcs/classes/Webservers/Webserver.cpp \
		srcs/classes/Webservers/WebserverManager.cpp srcs/classes/Config.cpp srcs/classes/MimeTypes.cpp \
		srcs/classes/HTTPCodes.cpp srcs/classes/ConfigUtils.cpp srcs/z_array/z_array_utils.cpp srcs/z_array/z_array.cpp

CGISRC = srcs/mainCGI.cpp
CGI    = cgi/my_cgi.test

OBJS = $(addprefix $(DIR_OBJS)/,$(SRCS:.cpp=.o))

DEPS = $(OBJS:%.o=%.d)

DIR_OBJS = objs

OBJECTS = $(sort $(dir $(OBJS)))

CC = c++ -g -fsanitize=address

FLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic-errors -I./includes

RM = rm -Rf

all: $(NAME)

$(NAME): $(OBJECTS) $(OBJS) Makefile
	@$(CC) $(CGISRC) -I./includes -o $(CGI)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS)
	@printf "\e[37;5;44m             Webserver is ready 🤗              \e[0m\n"

$(OBJECTS): $(DIR_OBJS)
	@mkdir -p $@

$(DIR_OBJS):
	@mkdir -p $(DIR_OBJS)

$(DIR_OBJS)/%.o:%.cpp
	@$(CC) $(FLAGS) -MMD -o $@ -c $<

-include $(DEPS)

clean:
	@$(RM) $(DIR_OBJS) $(CGI)
	@printf "\e[30;5;47m                 Clean done 🧹                  \e[0m\n"
	
fclean: clean
	@$(RM) $(NAME)
	@printf "\e[30;5;42m                Fclean done 🧹                  \e[0m\n"

re: fclean all

.PHONY:	all clean fclean re
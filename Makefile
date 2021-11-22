
NAME = webserv

CXX= clang++
CPPFLAGS= -g -Wall -Wextra -Werror

RED= \x1B[31m
GREEN= \033[92m
CYAN= \x1B[36m
ENDCOLOR= \x1B[0m

SRCS = main.cpp

SRCS_PARSER = $(addprefix Config_parser/, Parser.cpp)
SRCS_DEBUG = $(addprefix Debug.class/, Debug.cpp)
SRCS_CGI = $(addprefix cgi/, cgi.cpp)

SRCS_IHTTP = $(addprefix IHTTPMessage.interface/, Methods.cpp ${SRCS_REQUEST} ${SRCS_RESPONSE})
SRCS_REQUEST = $(addprefix Request.class/, Request.cpp)
SRCS_RESPONSE = $(addprefix Response.class/, Response.cpp)

SRCS_SERVER = $(addprefix Server.class/, Server.cpp Route.cpp ServersFamily.cpp ${SRCS_EXCEPTION} $(SRCS_UTILS))
SRCS_EXCEPTION = $(addprefix ServerException/, ServerException.cpp)
SRCS_UTILS = $(addprefix UtilsServer/, PollStruct.cpp)

SRCS_WEBVSERV = $(addprefix Webserver.class/, Webserver.cpp)

INCLUDES_STRUCTS = $(addprefix IHTTPMessage.interface/HTTPStructs/, s_bodies.hpp s_headers.hpp s_startline.hpp)

OBJS = $(addprefix $(BIN_DIR), $(SRCS:.cpp=.o) $(SRCS_PARSER:.cpp=.o) $(SRCS_CGI:.cpp=.o) $(SRCS_DEBUG:.cpp=.o) $(SRCS_IHTTP:.cpp=.o) $(SRCS_SERVER:.cpp=.o) $(SRCS_WEBVSERV:.cpp=.o))

SRCS_DIR= srcs/
BIN_DIR= bin/
WEBSERV_DIR= root/

BIN= $(shell mkdir -p $(addprefix $(BIN_DIR), $(dir $(SRCS_PARSER)) $(dir $(SRCS_CGI)) $(dir $(SRCS_DEBUG)) $(dir $(SRCS_IHTTP)) $(dir $(SRCS_SERVER)) $(dir $(SRCS_WEBVSERV))))

INCLUDES = $(addprefix srcs/, $(SRCS_PARSER:.cpp=.hpp) $(SRCS_DEBUG:.cpp=.hpp) $(SRCS_IHTTP:.cpp=.hpp) $(SRCS_SERVER:.cpp=.hpp) $(SRCS_WEBVSERV:.cpp=.hpp) $(INCLUDES_STRUCTS) )
RM = rm -rf

all: $(BIN) $(NAME)

$(BIN_DIR)%.o: $(SRCS_DIR)%.cpp
	$(CXX) -g $(CPPFLAGS) -MMD -c $< -o $@
#	-include $(OBJS:.o=.d)

$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) $^ -o $(addprefix $(WEBSERV_DIR), $@)
	$(RM) $(OBJS:.o=.d)
	echo "webserv ${GREEN}is compiled${ENDCOLOR}"

prepare:
	mkdir -p $(webservRoot)post_body
	mkdir -p $(WEBSERV_DIR)put_test
	mkdir -p $(WEBSERV_DIR)directory
	mkdir -p $(WEBSERV_DIR)YoupiBanane
	echo > $(WEBSERV_DIR)YoupiBanane/youbi.bla

test: prepare
	./$(WEBSERV_DIR)$(NAME) ../Configs/g_doggy.conf

clean:
	$(RM) $(OBJS)
	$(RM) $(BIN_DIR)
	echo "object files ${RED}are deleted${ENDCOLOR}"

fclean: clean
	$(RM) $(addprefix ${WEBSERV_DIR}, $(NAME))
	echo "webserv ${RED}are deleted${ENDCOLOR}"

re: fclean
	$(MAKE) all

.SILENT:

.PHONY: all clean fclean re


#  main.cpp
#  Server.class/UtilsServer/PollStruct.cpp
#  Server.class/ServerException/ServerException.cpp
#  Debug.class/Debug.cpp
#  IHTTPMessage.interface/Request.class/Request.cpp
#  IHTTPMessage.interface/Response.class/Response.cpp
#  IHTTPMessage.interface/Methods.cpp
#  Server.class/Route.cpp
#  Server.class/Server.cpp
#  Config_parser/Parser.cpp
#  cgi/cgi.cpp
#  Webserver.class/Webserver.cpp
#  Server.class/ServersFamily.cpp
NAME = $(WEBSERV_DIR)webserv

CXX= clang++
CPPFLAGS= -g -Wall -Wextra -Werror

RM = rm -rf

# COLOURS
RED= \x1B[31m
GREEN= \033[92m
CYAN= \x1B[36m
ENDCOLOR= \x1B[0m

# DIRECTORIES
SRCS_DIR= srcs/
BIN_DIR= bin/
WEBSERV_DIR= root/

# SOURCES
SRCS = main.cpp  $(SRCS_PARSER) $(SRCS_CGI) $(SRCS_DEBUG) $(SRCS_IHTTP) $(SRCS_SERVER) $(SRCS_WEBVSERV)

SRCS_PARSER = Config_parser/Parser.cpp
SRCS_DEBUG = Debug.class/Debug.cpp
SRCS_CGI = cgi/cgi.cpp
SRCS_WEBVSERV = Webserver.class/Webserver.cpp

SRCS_IHTTP = $(addprefix IHTTPMessage.interface/, Methods.cpp ${SRCS_REQUEST} ${SRCS_RESPONSE})
SRCS_REQUEST = Request.class/Request.cpp
SRCS_RESPONSE = Response.class/Response.cpp

SRCS_SERVER = $(addprefix Server.class/, Server.cpp Route.cpp ServersFamily.cpp ${SRCS_EXCEPTION} $(SRCS_UTILS))
SRCS_EXCEPTION = ServerException/ServerException.cpp
SRCS_UTILS = UtilsServer/PollStruct.cpp

# FILES and FOLDERS for TESTER
TESTER_DIR= put_test directory YoupiBanane/Yeah YoupiBanane/nop save_path
TESTER_FILES= $(addprefix $(WEBSERV_DIR)YoupiBanane/, youpi.bla youpi.bad_extension /Yeah/not_happy.bad_extension /nop/other.pouic nop/youpi.bad_extension)

# INCLUDES
INCLUDES_STRUCTS = $(addprefix IHTTPMessage.interface/HTTPStructs/, s_bodies.hpp s_headers.hpp s_startline.hpp)
INCLUDES = $(addprefix srcs/, $(filter-out main.hpp, $(SRCS:.cpp=.hpp)) $(INCLUDES_STRUCTS) )

# OBJECT FILES
OBJS = $(addprefix $(BIN_DIR), $(SRCS:.cpp=.o))

all: $(NAME)

$(BIN_DIR)%.o: $(SRCS_DIR)%.cpp $(INCLUDES)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -MMD -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) $^ -o $@
	echo "webserv ${GREEN}is compiled${ENDCOLOR}"

prepare:
	mkdir -p $(addprefix $(WEBSERV_DIR), $(TESTER_DIR))
	$(foreach var, $(TESTER_FILES), echo > $(var);)

run: prepare
	./$(NAME) Configs/g_doggy.conf

clean:
	$(RM) $(BIN_DIR)
	echo "object files ${RED}are deleted${ENDCOLOR}"

fclean:
	$(MAKE) clean
	$(RM) $(NAME)
	echo "webserv ${RED}are deleted${ENDCOLOR}"

re: fclean
	$(MAKE) all

#.SILENT:

.PHONY: all clean fclean re

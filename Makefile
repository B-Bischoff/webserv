VPATH = ./: \
		srcs/: \
		srcs/response/: \
		srcs/request/: \
		srcs/server/: \
		includes/: \
		srcs/ManageRequest/: \
		srcs/ManageRequest/RequestCheck/: \
		srcs/ManageRequest/RequestCheck/get/: \
		srcs/ManageRequest/RequestCheck/post/: \
		srcs/ManageRequest/RequestCheck/delete/: 
		

SRCS = 	webserv.cpp \
		Server.cpp  \
		ManageRequest.cpp \
		ResponseHeader.cpp \
		RequestHeader.cpp \
		Get.cpp \
		Post.cpp \
		Delete.cpp 

OBJS = $(addprefix .objects/, $(SRCS:.cpp=.o))

INCLUDES =	webserv.hpp \
			ResponseHeader.hpp \
			Server.hpp \
			ManageRequest.hpp \
			RequestHeader.hpp \
			RequestCheck.hpp \
			Get.hpp \
			Post.hpp \
			Delete.hpp
			

CC = g++

CFLAGS = -Wall -Wextra -Werror

DEBUG_FLAGS = #-fsanitize=address -g3

NAME = webserv

IINCLUDES = -Iincludes -Isrcs/server -Isrcs/response -Isrcs/request -Isrcs/RequestCheck -Isrcs/RequestCheck/Get -Isrcs/RequestCheck/Post -Isrcs/RequestCheck/Delete

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES) Makefile
	@$(CC) $(OBJS) ${CFLAGS} ${IINCLUDES} -o $@
	@echo "$(ERASE)$(GREEN)[CREATED $(NAME)]$(END)"

.objects/%.o:	%.cpp Makefile $(INCLUDES)
	@mkdir -p .objects
	@$(CC) $(CFLAGS) ${IINCLUDES} -c $< -o $@
	@printf "$(ERASE)$(BLUE)[BUILDING]$(END) $@"

clean:
	@${RM} ${OBJS}
	@echo "${RED}[DELETED WEBSERV OBJS]${END}"

fclean:	clean
	@${RM} ${NAME}
	@${RM} .objects
	@echo "${RED}[DELETED]${END} ${NAME}"

re:	fclean all
VPATH = ./: \
		srcs/: \
		srcs/response/: \
		srcs/request/: \
		srcs/server/: \
		includes/: \
		srcs/ManageRequest/: \
		srcs/ManageRequest/Method/: \
		srcs/ManageRequest/Method/get/: \
		srcs/ManageRequest/Method/post/: \
		srcs/ManageRequest/Method/delete/: \
		srcs/Status/:
		

SRCS = 	webserv.cpp \
		Server.cpp  \
		VirtualServer.cpp  \
		ManageRequest.cpp \
		ResponseHeader.cpp \
		RequestHeader.cpp \
		Method.cpp \
		Get.cpp \
		Post.cpp \
		Delete.cpp \
		ErrorStatus.cpp

OBJS = $(addprefix .objects/, $(SRCS:.cpp=.o))

INCLUDES =	webserv.hpp \
			ResponseHeader.hpp \
			Server.hpp \
			VirtualServer.hpp \
			ManageRequest.hpp \
			RequestHeader.hpp \
			Method.hpp \
			Get.hpp \
			Post.hpp \
			Delete.hpp \
			ErrorStatus.hpp
			

CC = g++

CFLAGS = -Wall -Wextra -Werror

DEBUG_FLAGS = #-fsanitize=address -g3

NAME = webserv

LINK := ${addprefix -I, ${subst /:,,${VPATH}}}

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES) Makefile
	@$(CC) $(OBJS) ${CFLAGS} -o $@
	@echo "$(ERASE)$(GREEN)[CREATED $(NAME)]$(END)"

.objects/%.o:	%.cpp Makefile $(INCLUDES)
	@mkdir -p .objects
	@$(CC) $(CFLAGS) ${LINK} -c $< -o $@
	@printf "$(ERASE)$(BLUE)[BUILDING]$(END) $@"

clean:
	@${RM} ${OBJS}
	@echo "${RED}[DELETED WEBSERV OBJS]${END}"

fclean:	clean
	@${RM} ${NAME}
	@${RM} .objects
	@echo "${RED}[DELETED]${END} ${NAME}"

re:	fclean all
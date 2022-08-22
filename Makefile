# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tarchimb <tarchimb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/17 11:24:39 by tarchimb          #+#    #+#              #
#    Updated: 2022/08/19 17:12:34 by tarchimb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	Server.cpp				\
				ResponseHeader.cpp		

OBJS		=	$(addprefix ${OBJS_DIR}/, ${SRCS:.cpp=.o})

OBJS_DIR	=	objects
SRCS_DIR	=	srcs

INCLUDES	=	./includes/Server.hpp			\
				./includes/webserv.hpp			\
				./includes/ResponseHeader.hpp	

CC			= 	c++
CFLAGS		= 	-Wall -Wextra -Werror -g #-fsanitize=address

LIBINCLUDES	=	-Iincludes -Ilibft/includes

LIBS		=	${LIB}							

LIB			=	libwebserv.a						

NAME		=	webserv

RM			=	rm -rf	

MKDIR		=	mkdir -p

all:		libft ${NAME}

${OBJS_DIR}/%.o: 	${SRCS_DIR}/%.cpp	${INCLUDES} $(OBJDIRS)
			${MKDIR} $(@D) $(DMPDIR)$(@D)
			${CC} ${CFLAGS} ${LIBINCLUDES} -c $< -o $@

$(LIB):		${OBJS} ${INCLUDES}
			ar rcs ${LIB} ${OBJS}

libft:		${INCLUDES}

${NAME}:	${NAME}.cpp ${LIBS}
			$(CC) ${CFLAGS} ${LIBINCLUDES} ${NAME}.cpp ${LIBS} -o ${NAME}

clean:
			${RM} ${OBJS} $(OBJS_DIR)

fclean:		clean
			${RM} ${LIB} ${NAME} ${LIBS} ${NAME}.dSYM

re:			fclean all

.PHONY: 	all clean fclean re object
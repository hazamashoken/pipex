# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/14 00:32:16 by tliangso          #+#    #+#              #
#    Updated: 2022/09/30 18:14:11 by tliangso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIRSRC	=	./

SRC		=	pipex.c utils.c ft_split.c


BONUS	=	pipex_bonus.c get_next_line.c get_next_line_utils.c utils.c ft_split.c

EXTRA	=

SRCS	= ${addprefix ${DIRSRC}, ${SRC}}

BONUS_SRCS = ${addprefix ${DIRSRC}, ${BONUS}}

EXTRA_SRCS = ${addprefix ${DIRSRC}, ${EXTRA}}

OBJS	= ${SRCS:.c=.o}

BONUS_OBJS = ${BONUS_SRCS:.c=.o}

EXRTA_OBJS = ${EXTRA_SRCS:.c=.o}

HEAD	= .

NAME	= pipex

CC		= gcc

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror

.c.o:
			${CC} ${CFLAGS} -c -I ${HEAD} $< -o ${<:.c=.o}

all:		${NAME}

${NAME}:	${OBJS}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS}

bonus:		${BONUS_OBJS}
			${CC} ${CFLAGS} -o ${NAME} ${BONUS_OBJS}

extra:	${OBJS} ${BONUS_OBJS} ${EXRTA_OBJS}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${BONUS_OBJS} ${EXRTA_OBJS}

clean:
			${RM} ${OBJS} ${BONUS_OBJS} ${EXRTA_OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean	all

b:	bonus clean

m:	${NAME} clean

.PHONY:		all	clean	fclean	re bonus b m

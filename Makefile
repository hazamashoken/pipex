# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/14 00:32:16 by tliangso          #+#    #+#              #
#    Updated: 2022/10/01 21:34:52 by tliangso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### DIR ###
DIRSRC	=	src/
BONUS_SRC =	bonus/
HEAD	= include/

### TESTER GIT URL ###
TESTER1 = "git@github.com:vfurmane/pipex-tester.git"
TESTER2 = "git@github.com:hazamashoken/PIPEX_TESTER.git"
TESTER3 = "git@github.com:Yoo0lh/pipex_tester_42.git"

### SOURCE FILE ###
SRC		=	pipex.c utils.c utils2.c ft_split.c free.c path_finder.c sanitiser.c

BONUS	=	pipex_bonus.c get_next_line.c get_next_line_utils.c utils_bonus.c \
			utils2_bonus.c ft_split_bonus.c here_doc_bonus.c msg_handler_bonus.c \
			pipe_tools_bonus.c free_bonus.c sanitiser_bonus.c path_finder_bonus.c \


EXTRA	=

### PATH ###
SRCS	= ${addprefix ${DIRSRC}, ${SRC}}
BONUS_SRCS = ${addprefix ${BONUS_SRC}, ${BONUS}}
EXTRA_SRCS = ${addprefix ${DIRSRC}, ${EXTRA}}

### OBJECT FILE ###
OBJS	= ${SRCS:.c=.o}
BONUS_OBJS = ${BONUS_SRCS:.c=.o}
EXRTA_OBJS = ${EXTRA_SRCS:.c=.o}

### EXECUTABLE ###
NAME	= pipex

### COMPILATION ###
CC		= gcc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror

### COLORS ###
NOC		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
WHITE	= \033[1;37m

### RULES ###
.c.o:
	@${CC} ${CFLAGS} -c -I ${HEAD} $< -o ${<:.c=.o}
	@echo "$(BLUE)clang $(WHITE)$(notdir $@)$(NOC)"

all: ${NAME}

${NAME}:	${OBJS}

	@${CC} ${CFLAGS} -o ${NAME} ${OBJS}
	@echo "$(GREEN)$@$(NOC)"

bonus:		${BONUS_OBJS}
	@${CC} ${CFLAGS} -o ${NAME} ${BONUS_OBJS}
	@echo "$(GREEN)$@$(NOC)"

extra:	${OBJS} ${BONUS_OBJS} ${EXRTA_OBJS}
	@${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${BONUS_OBJS} ${EXRTA_OBJS}

clean:
	@echo "$(RED)clean$(NOC)"
	@${RM} ${OBJS} ${BONUS_OBJS} ${EXRTA_OBJS}

fclean: clean
	@echo "$(RED)fclean$(NOC)"
	@${RM} ${NAME}

re: fclean	all

b: bonus clean

m: ${NAME} clean

g: ${BONUS_OBJS}
	@${CC} -g ${CFLAGS} -o ${NAME} ${BONUS_OBJS}

tester:
	git clone ${TESTER1} TESTER1
	git clone ${TESTER2} TESTER2
	git clone ${TESTER3} TESTER3

norm:
	norminette

gitpush:
	git add .
	git status
	git commit -m ${NAME}
	git push

.PHONY:		all	clean	fclean	re bonus b m g norm gitpush

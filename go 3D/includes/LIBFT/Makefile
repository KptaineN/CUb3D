# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkief <nkief@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/20 11:11:03 by nkiefer           #+#    #+#              #
#    Updated: 2025/09/21 18:28:35 by nkief            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = FCT/ft_isalpha.c \
       FCT/ft_isdigit.c \
       FCT/ft_isalnum.c \
       FCT/ft_isprint.c \
       FCT/ft_strlen.c \
       FCT/ft_memset.c \
       FCT/ft_bzero.c \
       FCT/ft_memcpy.c \
       FCT/ft_memmove.c \
       FCT/ft_strlcpy.c \
       FCT/ft_strlcat.c \
       FCT/ft_toupper.c \
       FCT/ft_tolower.c \
       FCT/ft_strchr.c \
       FCT/ft_strrchr.c \
       FCT/ft_strstr.c \
       FCT/ft_strncmp.c \
       FCT/ft_memchr.c \
       FCT/ft_memcmp.c \
       FCT/ft_strnstr.c \
       FCT/ft_calloc.c \
       FCT/ft_atoi.c \
       FCT/ft_striteri.c \
       FCT/ft_putchar_fd.c \
       FCT/ft_putstr_fd.c \
       FCT/ft_putendl_fd.c \
       FCT/ft_putnbr_fd.c \
       FCT/ft_strmapi.c \
       FCT/ft_itoa.c \
       FCT/ft_strdup.c \
       FCT/ft_isascii.c \
       FCT/ft_strtrim.c \
       FCT/ft_substr.c \
       FCT/ft_strjoin.c \
       FCT/ft_split.c \
       FCT/ft_atol.c \
       FCT/ft_isspace.c \
       FCT/ft_skip_spaces.c \
       FCT/ft_swap.c \
       FCT/ft_strcmp.c \
       FCT/ft_strcspn.c \
       FCT/ft_strcpy.c \
       FCT/ft_strndup.c \
       FCT/ft_strncpy.c \
       FCT/ft_bubble_str_sort.c \
       GNL/get_next_line.c \
       GNL/get_next_line_utils.c \
       GNL/get_next_line_utils.c \
       PRINTF/ft_printf.c \
       PRINTF/ft_print_num.c \
       PRINTF/ft_print_pointer.c \
       PRINTF/ft_print_f.c \


OBJS = ${SRCS:.c=.o}
CC = cc
RM = rm -f
AR = ar rcs
CFLAGS = -Wall -Wextra -Werror

all: ${NAME}
	
${NAME}: ${OBJS}
	$(AR) $@ $^
	
%.o: %.c	
	${CC} ${CFLAGS} -c $< -o $@

clean: 
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY : all clean fclean re so

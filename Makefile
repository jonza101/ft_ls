# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/07 15:27:41 by zjeyne-l          #+#    #+#              #
#    Updated: 2020/04/09 17:33:25 by zjeyne-l         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRC = main.c read.c output.c func.c
SRC += gnl/get_next_line.c gnl/libft/libft.a
INC = -I gnl/


all:
	gcc -g $(SRC) $(INC)

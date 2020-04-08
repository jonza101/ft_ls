/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 15:19:21 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/08 22:39:18 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include <stdlib.h>
#include <stdio.h>

#include "gnl/get_next_line.h"


#define COLOR_WHITE "\x1b[0m"
#define COLOR_CYAN "\x1b[36m"


typedef struct		s_flag
{
	int 			l;
	int 			a;
	int 			t;
	int				r;
	int				R;
}					t_flag;

typedef struct		s_elem
{
	char			*name;
	char			*path;

	struct s_elem	*next;
}					t_elem;

typedef struct		s_ls
{
	t_flag			*flag;

	int				*elem_idx;
	int				elem_count;

	int				dir_read_count;
}					t_ls;


void 				ft_read_args(t_ls *ls, int argc, char **argv);

void				ft_putstr_endc(char *str, char end_char);
void 				ft_output_elem(t_ls *ls, char *name, struct stat *stat_buffer);

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 15:19:21 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/09 21:45:20 by zjeyne-l         ###   ########.fr       */
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
#define COLOR_BLUE "\x1b[1;34m"
#define COLOR_YELLOW "\x1b[01;33m"
#define COLOR_GREEN "\x1b[1;32m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_MAGENTA "\x1b[1;35m"
#define COLOR_CYAN "\x1b[1;36m"

#define COL 4


typedef struct		s_flag
{
	int 			l;
	int 			a;
	int 			t;
	int				r;
	int				R;
	int 			G;
}					t_flag;

typedef struct		s_elem
{
	char			*name;
	char			*path;

	long long		time_diff;

	struct s_elem	*next;
}					t_elem;

typedef struct		s_ls
{
	t_flag			*flag;

	long long		curr_time;
	int 			curr_year;

	int				*elem_idx;
	int				elem_count;
	int 			dir_read;

	void			(*sort[2])(struct s_ls *ls, t_elem *elem_head);
}					t_ls;


void 				ft_read_args(t_ls *ls, int argc, char **argv);

void				ft_putstr_endc(char *str, char end_char);
void 				ft_output_elem(t_ls *ls, char *name, struct stat *stat_buffer);

void				ft_sort_time(t_ls *ls, t_elem *elem_head);
void				ft_sort_ascii(t_ls *ls, t_elem *elem_head);

void				ft_split_free(char **split);
long long 			ft_get_time(char *time_str, int *o_year, int *add_day);

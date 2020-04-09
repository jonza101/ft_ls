/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 23:19:30 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/09 21:28:59 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


void	ft_putstr_endc(char *str, char end_char)
{
	int len = ft_strlen(str);
	write(1, str, len);
	write(1, &end_char, 1);
}

void	ft_output_elem(t_ls *ls, char *name, struct stat *stat_buffer)
{
	if (name[0] == '.' && !ls->flag->a)
		return;

	int mode = stat_buffer->st_mode & __S_IFMT;
	char *color = COLOR_WHITE;

	if (ls->flag->G)
	{
	if (S_ISDIR(mode))
		color = COLOR_BLUE;
	if (S_ISLNK(mode))
		color = COLOR_YELLOW;
	if (S_ISBLK(mode))
		color = COLOR_GREEN;
	if (S_ISCHR(mode))
		color = COLOR_RED;
	if (S_ISSOCK(mode))
		color = COLOR_MAGENTA;
	if (S_ISFIFO(mode))
		color = COLOR_CYAN;
	}

	if (!ls->flag->l)
	{
		ft_putstr(color);
		ft_putstr_endc(name, '\t');
		ft_putstr(COLOR_WHITE);
		return;
	}

	char *permissions = ft_strnew(10);
	if (S_ISREG(mode))
		permissions[0] = '-';
	if (S_ISDIR(mode))
		permissions[0] = 'd';
	if (S_ISLNK(mode))
		permissions[0] = 'l';
	if (S_ISBLK(mode))
		permissions[0] = 'b';
	if (S_ISCHR(mode))
		permissions[0] = 'c';
	if (S_ISSOCK(mode))
		permissions[0] = 's';
	if (S_ISFIFO(mode))
		permissions[0] = 'p';

	permissions[1] = (stat_buffer->st_mode & S_IRUSR) ? 'r' : '-';
	permissions[2] = (stat_buffer->st_mode & S_IWUSR) ? 'w' : '-';
	permissions[3] = (stat_buffer->st_mode & S_IXUSR) ? 'x' : '-';
	permissions[4] = (stat_buffer->st_mode & S_IRGRP) ? 'r' : '-';
	permissions[5] = (stat_buffer->st_mode & S_IWGRP) ? 'w' : '-';
	permissions[6] = (stat_buffer->st_mode & S_IXGRP) ? 'x' : '-';
	permissions[7] = (stat_buffer->st_mode & S_IROTH) ? 'r' : '-';
	permissions[8] = (stat_buffer->st_mode & S_IWOTH) ? 'w' : '-';
	permissions[9] = (stat_buffer->st_mode & S_IXOTH) ? 'x' : '-';

	char *links = ft_itoa(stat_buffer->st_nlink);

	struct passwd *pwd = getpwuid(stat_buffer->st_uid);
	char *uid = ft_strdup(pwd->pw_name);

	struct group *grp = getgrgid(stat_buffer->st_gid);
	char *gid = ft_strdup(grp->gr_name);

	char *size = ft_itoa(stat_buffer->st_size);

	
	char *mod_time_s = ctime(&stat_buffer->st_mtime) + 4;
	char *mod_time = ft_strnew(12);
	int i = -1;
	while (++i < 12)
		mod_time[i] = mod_time_s[i];

	ft_putstr_endc(permissions, '\t');
	ft_putstr_endc(links, '\t');
	ft_putstr_endc(uid, '\t');
	ft_putstr_endc(gid, '\t');
	ft_putstr_endc(size, '\t');
	ft_putstr_endc(mod_time, '\t');
	ft_putstr(color);
	ft_putstr_endc(name, '\n');
	ft_putstr(COLOR_WHITE);

	free(permissions);
	free(links);
	free(uid);
	free(gid);
	free(size);
	free(mod_time);
}

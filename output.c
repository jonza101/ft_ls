/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 23:19:30 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/09 19:46:40 by zjeyne-l         ###   ########.fr       */
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

	int is_directory = (S_ISDIR(stat_buffer->st_mode));

	if (!ls->flag->l)
	{
		(is_directory) ? ft_putstr(COLOR_CYAN) : ft_putstr(COLOR_WHITE);
		ft_putstr_endc(name, '\t');
		ft_putstr(COLOR_WHITE);
		return;
	}

	char *permissions = ft_strnew(10);
	permissions[0] = '-';

	int mode = stat_buffer->st_mode;
	if (is_directory)
		permissions[0] = 'd';
	else if (S_ISLNK(mode))
		permissions[0] = 'l';
	else if (S_ISBLK(mode))
		permissions[0] = 'b';
	else if (S_ISCHR(mode))
		permissions[0] = 'c';
	else if (S_ISFIFO(mode))
		permissions[0] = 'p';
	else if (S_ISSOCK(mode))
		permissions[0] = 's';

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
	(is_directory) ? ft_putstr(COLOR_CYAN) : ft_putstr(COLOR_WHITE);
	ft_putstr_endc(name, '\n');
	ft_putstr(COLOR_WHITE);

	free(permissions);
	free(links);
	free(uid);
	free(gid);
	free(size);
	free(mod_time);
}

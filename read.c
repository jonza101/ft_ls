/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 23:17:45 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/11 16:31:20 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


void	ft_read_dir(t_ls *ls, char *path)
{
	DIR *dir;
	struct dirent *entry;

	struct stat stat_buffer;

	t_elem *elem = NULL;
	t_elem *elem_head = NULL;
	int elem_count = 0;

	int total = 0;

	dir = opendir(path);
	if (!dir)
	{
		char *err = ft_strjoin("Unable to open directory: ", path);
		int len = ft_strlen(err);
		write(1, err, len);
		write(1, "\n", 1);
		free(err);
		return;
	}

	if (ls->elem_count > 1 || ls->flag->R)
	{
		(ls->dir_read) ? write(1, "\n", 1) : 1;
		char *path_str = ft_strjoin(path, ":");
		ft_putstr_endc(path_str, '\n');
		free(path_str);
	}

	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] == '.' && !ls->flag->a)
			continue;
		int path_len = ft_strlen(path);
		char *elem_path_s = (path[path_len - 1] != '/') ? ft_strjoin(path, "/") : ft_strdup(path);
		char *elem_path = ft_strjoin(elem_path_s, entry->d_name);
		free(elem_path_s);

		lstat(elem_path, &stat_buffer);
		total += stat_buffer.st_blocks;

		if (!elem_count)
		{
			elem = (t_elem*)malloc(sizeof(t_elem));
			elem->name = ft_strdup(entry->d_name);
			elem->path = ft_strdup(elem_path);

			int add_day, year;
			char *mod_time_s = ctime(&stat_buffer.st_mtime) + 4;
			elem->time_diff = ft_get_time(mod_time_s, &year, &add_day);
			elem->time_diff = ls->curr_time - (elem->time_diff + (ls->curr_year - year) * 60 * 60 * 24 * (30 + add_day) * 365);

			elem->next = NULL;
			elem_head = elem;
		}
		else
		{
			elem->next = (t_elem*)malloc(sizeof(t_elem));
			elem->next->name = ft_strdup(entry->d_name);
			elem->next->path = ft_strdup(elem_path);

			int add_day, year;
			char *mod_time_s = ctime(&stat_buffer.st_mtime) + 4;
			elem->next->time_diff = ft_get_time(mod_time_s, &year, &add_day);
			elem->next->time_diff = ls->curr_time - (elem->next->time_diff + (ls->curr_year - year) * 60 * 60 * 24 * (30 + add_day) * 365);

			elem = elem->next;
			elem->next = NULL;
		}
		elem_count++;

		free(elem_path);
	}
	closedir(dir);

	if (ls->flag->l)
	{
		char *total_str_s = ft_itoa(total);
		char *total_str = ft_strjoin("total ", total_str_s);
		ft_putstr_endc(total_str, '\n');
		free(total_str_s);
		free(total_str);
	}

	ls->sort[ls->flag->t](ls, elem_head);

	int col = 0;
	elem = elem_head;
	while (elem)
	{
		lstat(elem->path, &stat_buffer);
		ft_output_elem(ls, elem->name, &stat_buffer);
		elem = elem->next;
		col++;
		if (!ls->flag->l && col == COL)
		{
			write(1, "\n", 1);
			col = 0;
		}
	}
	if (!ls->flag->l && col)
		write(1, "\n", 1);

	if (!ls->flag->R)
		return;
	elem = elem_head;
	while (elem)
	{
		lstat(elem->path, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode))
		{
			if (ft_strcmp(elem->name, ".") && ft_strcmp(elem->name, ".."))
			{
				//write(1, "\n", 1);
				ls->dir_read++;
				ft_read_dir(ls, elem->path);
			}
		}
		elem = elem->next;
	}
}

void	ft_read_elems(t_ls *ls, char **argv)
{
	struct stat stat_buffer;

	t_elem *elem = NULL;
	t_elem *elem_head = NULL;
	int elem_count = 0;
	int dir_count = 0;
	int file_count = 0;

	int i = -1;
	while (++i < ls->elem_count)
	{
		lstat(argv[ls->elem_idx[i]], &stat_buffer);
		dir_count += S_ISDIR(stat_buffer.st_mode);

		if (!elem_count)
		{
			elem = (t_elem*)malloc(sizeof(t_elem));
			elem->name = ft_strdup(argv[ls->elem_idx[i]]);

			int add_day, year;
			char *mod_time_s = ctime(&stat_buffer.st_mtime) + 4;
			elem->time_diff = ft_get_time(mod_time_s, &year, &add_day);
			elem->time_diff = ls->curr_time - (elem->time_diff + (ls->curr_year - year) * 60 * 60 * 24 * (30 + add_day) * 365);

			elem->next = NULL;
			elem_head = elem;
		}
		else
		{
			elem->next = (t_elem*)malloc(sizeof(t_elem));
			elem->next->name = ft_strdup(argv[ls->elem_idx[i]]);

			int add_day, year;
			char *mod_time_s = ctime(&stat_buffer.st_mtime) + 4;
			elem->next->time_diff = ft_get_time(mod_time_s, &year, &add_day);
			elem->next->time_diff = ls->curr_time - (elem->next->time_diff + (ls->curr_year - year) * 60 * 60 * 24 * (30 + add_day) * 365);

			elem = elem->next;
			elem->next = NULL;
		}
		elem_count++;
	}

	ls->sort[ls->flag->t](ls, elem_head);
	int col = 0;
	elem = elem_head;
	while (elem)
	{
		lstat(elem->name, &stat_buffer);
		if (!S_ISDIR(stat_buffer.st_mode))
		{
			ft_output_elem(ls, elem->name, &stat_buffer);
			file_count++;
			col++;
			if (!ls->flag->l && col == COL)
			{
				write(1, "\n", 1);
				col = 0;
			}
		}
		elem = elem->next;
	}
	if (!ls->flag->l && file_count && col)
		write(1, "\n", 1);

	if (!dir_count)
		return;
	if (file_count)
		write(1, "\n", 1);

	elem = elem_head;
	while (elem)
	{
		lstat(elem->name, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode))
		{
			if (ft_strcmp(elem->name, ".") && ft_strcmp(elem->name, ".."))
			{
				//write(1, "\n", 1);
				ft_read_dir(ls, elem->name);
				ls->dir_read++;
			}
		}
		elem = elem->next;
	}
}

void	ft_read_flags(t_ls *ls, char *str)
{
	int i = -1;
	while (str[++i])
	{
		if (str[i] == 'l')
			ls->flag->l = 1;
		else if (str[i] == 'a')
			ls->flag->a = 1;
		else if (str[i] == 't')
			ls->flag->t = 1;
		else if (str[i] == 'r')
			ls->flag->r = 1;
		else if (str[i] == 'R')
			ls->flag->R = 1;
		else if (str[i] == 'G')
			ls->flag->G = 1;
	}
}

void	ft_read_args(t_ls *ls, int argc, char **argv)
{
	ls->elem_idx = (int*)malloc(sizeof(int) * (argc - 1));
	ls->elem_count = 0;
	ls->dir_read = 0;
	struct stat stat_buffer;
	int corr = 1;

	int i = 0;
	while (++i < argc)
	{
		if (stat(argv[i], &stat_buffer) == 0)
		{
			ls->elem_idx[ls->elem_count] = i;
			ls->elem_count++;
		}
		else if (argv[i][0] == '-')
			ft_read_flags(ls, argv[i]);
		else
		{
			corr = 0;
			char *err = ft_strjoin(argv[i], ": No such file or directory\n");
			int len = ft_strlen(err);
			write(1, err, len);
			free(err);
		}
	}

	(!ls->elem_count && corr) ? ft_read_dir(ls, ".") : ft_read_elems(ls, argv);
}

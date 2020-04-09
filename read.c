/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 23:17:45 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/09 19:44:55 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


void	ft_sort_time(t_ls *ls, t_elem *elem_head)
{
	t_elem *elem = elem_head;
	while (elem)
	{
		t_elem *nxt = elem->next;
		while (nxt)
		{
			int elem_cmp = ft_strcmp(elem->name, nxt->name);
			int e_time = elem->time_diff;
			int n_time = nxt->time_diff;
			if (ls->flag->r)
			{
				e_time = -e_time;
				n_time = -n_time;
			}
			if (e_time > n_time)
			{
				char *temp_name = elem->name;
				char *temp_path = elem->path;
				long long temp_time = elem->time_diff;
				elem->name = nxt->name;
				elem->path = nxt->path;
				elem->time_diff = nxt->time_diff;
				nxt->name = temp_name;
				nxt->path = temp_path;
				nxt->time_diff = temp_time;
			}
			
			nxt = nxt->next;
		}
		elem = elem->next;
	}
}

void	ft_sort_ascii(t_ls *ls, t_elem *elem_head)
{
	t_elem *elem = elem_head;
	while (elem)
	{
		t_elem *nxt = elem->next;
		while (nxt)
		{
			int elem_cmp = ft_strcmp(elem->name, nxt->name);
			elem_cmp = (ls->flag->r) ? -elem_cmp : elem_cmp;
			if (elem_cmp > 0) 				//	NOT SURE ABOUT IT
			{
				char *temp_name = elem->name;
				char *temp_path = elem->path;
				elem->name = nxt->name;
				elem->path = nxt->path;
				nxt->name = temp_name;
				nxt->path = temp_path;
			}
			
			nxt = nxt->next;
		}
		elem = elem->next;
	}
}

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
		return;

	if (ls->elem_count || ls->flag->R)
	{
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

		stat(elem_path, &stat_buffer);
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

	elem = elem_head;
	while (elem)
	{
		stat(elem->path, &stat_buffer);
		ft_output_elem(ls, elem->name, &stat_buffer);
		elem = elem->next;
	}
	if (!ls->flag->l)
		write(1, "\n", 1);

	if (!ls->flag->R)
		return;
	elem = elem_head;
	while (elem)
	{
		stat(elem->path, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode))
		{
			if (ft_strcmp(elem->name, ".") && ft_strcmp(elem->name, ".."))
			{
				write(1, "\n", 1);
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

	int i = -1;
	while (++i < ls->elem_count)
	{
		stat(argv[ls->elem_idx[i]], &stat_buffer);
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
	elem = elem_head;
	while (elem)
	{
		stat(elem->name, &stat_buffer);
		if (!S_ISDIR(stat_buffer.st_mode))
			ft_output_elem(ls, elem->name, &stat_buffer);
		elem = elem->next;
	}
	if (!ls->flag->l)
		write(1, "\n", 1);

	if (!dir_count)
		return;

	elem = elem_head;
	while (elem)
	{
		stat(elem->name, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode))
		{
			if (ft_strcmp(elem->name, ".") && ft_strcmp(elem->name, ".."))
			{
				write(1, "\n", 1);
				ft_read_dir(ls, elem->name);
			}
		}
		elem = elem->next;
	}
}

void	ft_read(t_ls *ls, char **argv)
{
	ft_read_dir(ls, ".");
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
	}
}

void	ft_read_args(t_ls *ls, int argc, char **argv)
{
	ls->elem_idx = (int*)malloc(sizeof(int) * (argc - 1));
	ls->elem_count = 0;
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

	(!ls->elem_count && corr) ? ft_read(ls, argv) : ft_read_elems(ls, argv);
}

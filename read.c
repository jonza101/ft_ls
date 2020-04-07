/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 23:17:45 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/08 00:00:40 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_read_dir(t_ls *ls, char *dir_path)
{
	DIR *dir;
	struct dirent *entry;

	t_elem *e_path = NULL;
	t_elem *e_path_head = NULL;
	int e_path_count = 0;

	struct stat stat_buffer;
	dir = opendir(dir_path);
	if (!dir)
		return;

	if (ls->flag->R)
	{
		char *dir_path_str = ft_strjoin(dir_path, ":");
		ft_putstr(COLOR_WHITE);
		ft_putstr_endc(dir_path_str, '\n');
		ft_putstr(COLOR_WHITE);

		free(dir_path_str);
	}

	int total = 0;

	while ((entry = readdir(dir)) != NULL)
	{
		char *elem_path_s = ft_strjoin(dir_path, "/");
		char *elem_path = ft_strjoin(elem_path_s, entry->d_name);
		free(elem_path_s);

		if (stat(elem_path, &stat_buffer) == 0)
		{
			int is_directory = S_ISDIR(stat_buffer.st_mode);
			ft_output_elem(ls, entry->d_name, &stat_buffer);
			if (is_directory && ls->flag->R)
			{
				if (ls->flag->R && entry->d_name[0] != '.')
				{
					if (e_path_count == 0)
					{
						e_path = (t_elem *)malloc(sizeof(t_elem));
						e_path->name = ft_strdup(elem_path);
						e_path->next = NULL;
						e_path_head = e_path;
					}
					else
					{
						e_path->next = (t_elem *)malloc(sizeof(t_elem));
						e_path->next->name = ft_strdup(elem_path);
						e_path = e_path->next;
						e_path->next = NULL;
					}
					e_path_count++;
				}
			}
		}
		free(elem_path);
	}
	(!ls->flag->l) ? printf("\n\n") : printf("\n");

	e_path = e_path_head;
	t_elem *epath = e_path;
	while (epath)
	{
		ft_read_dir(ls, epath->name);
		epath = epath->next;
	}

	closedir(dir);
}

void	ft_read(t_ls *ls)
{
	DIR *dir;
	struct dirent *entry;

	char *root_path = "";

	t_elem *e_path = NULL;
	t_elem *e_path_head = NULL;
	int e_path_count = 0;

	t_elem *elem = ls->elem;
	while (elem)
	{
		char *elem_path = ft_strjoin(root_path, elem->name);

		struct stat stat_buffer;
		stat(elem_path, &stat_buffer);
		int is_directory = S_ISDIR(stat_buffer.st_mode);

		if (is_directory)
		{
			if (!ls->flag->R)
				ft_output_elem(ls, elem->name, &stat_buffer);
			if (e_path_count == 0)
			{
				e_path = (t_elem *)malloc(sizeof(t_elem));
				e_path->name = ft_strdup(elem_path);
				e_path->next = NULL;
				e_path_head = e_path;
			}
			else
			{
				e_path->next = (t_elem *)malloc(sizeof(t_elem));
				e_path->next->name = ft_strdup(elem_path);
				e_path = e_path->next;
				e_path->next = NULL;
			}
			e_path_count++;
		}
		else
			ft_output_elem(ls, elem->name, &stat_buffer);

		elem = elem->next;
		free(elem_path);
	}

	e_path = e_path_head;
	t_elem *epath = e_path;
	while (epath)
	{
		ft_read_dir(ls, epath->name);
		epath = epath->next;
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
	}
}

void	ft_read_args(t_ls *ls, int argc, char **argv)
{
	t_elem *elem_head;
	ls->elem_count = 0;

	struct stat stat_buffer;

	int i = 0;
	while (++i < argc)
	{
		if (stat(argv[i], &stat_buffer) == 0)
		{
			if (ls->elem_count == 0)
			{
				ls->elem = (t_elem *)malloc(sizeof(t_elem));
				ls->elem->name = ft_strdup(argv[i]);
				ls->elem->next = NULL;
				elem_head = ls->elem;
			}
			else
			{
				ls->elem->next = (t_elem *)malloc(sizeof(t_elem));
				ls->elem->next->name = ft_strdup(argv[i]);
				ls->elem = ls->elem->next;
				ls->elem->next = NULL;
			}
			ls->elem_count++;
		}
		else if (argv[i][0] == '-')
			ft_read_flags(ls, argv[i]);
		else
		{
			char *err = ft_strjoin(argv[i], ": No such file or directory\n");
			ft_putstr(err);
			free(err);
		}
	}
	ls->elem = elem_head;

	if (ls->elem_count == 0)
	{
		ls->elem = (t_elem *)malloc(sizeof(t_elem));
		ls->elem->name = ft_strdup(".");
		ls->elem->next = NULL;
	}
}

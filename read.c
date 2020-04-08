/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 23:17:45 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/08 22:46:23 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


void	ft_sort_ascii(t_ls *ls, t_elem *elem_head)
{
	t_elem *elem = elem_head;
	while (elem)
	{
		t_elem *nxt = elem->next;
		while (nxt)
		{
			if (ft_strcmp(elem->name, nxt->name) > 0)		//	NOT SURE ABOUT IT
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
		char *elem_path_s = ft_strjoin(path, "/");
		char *elem_path = ft_strjoin(elem_path_s, entry->d_name);
		free(elem_path_s);

		if (!elem_count)
		{
			elem = (t_elem*)malloc(sizeof(t_elem));
			elem->name = ft_strdup(entry->d_name);
			elem->path = ft_strdup(elem_path);
			elem->next = NULL;
			elem_head = elem;
		}
		else
		{
			elem->next = (t_elem*)malloc(sizeof(t_elem));
			elem->next->name = ft_strdup(entry->d_name);
			elem->next->path = ft_strdup(elem_path);
			elem = elem->next;
			elem->next = NULL;
		}
		elem_count++;

		free(elem_path);
	}
	closedir(dir);

	ft_sort_ascii(ls, elem_head);

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
		if (S_ISDIR(stat_buffer.st_mode) && elem->name[0] != '.')
		{
			write(1, "\n", 1);
			ft_read_dir(ls, elem->path);
		}
		elem = elem->next;
	}
	//write(1, "\n", 1);
}

// void	ft_read_elems(t_ls *ls, char **argv)
// {
	
// }

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
	ls->dir_read_count = 0;
	struct stat stat_buffer;

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
			char *err = ft_strjoin(argv[i], ": No such file or directory\n");
			ft_putstr(err);
			free(err);
		}
	}

	ft_read(ls, argv);
}

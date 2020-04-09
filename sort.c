/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 21:52:05 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/09 21:52:18 by zjeyne-l         ###   ########.fr       */
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

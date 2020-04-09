/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 15:17:42 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/09 19:00:26 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


int		main(int argc, char **argv)
{
	t_ls *ls = (t_ls*)malloc(sizeof(t_ls));
	ls->flag = (t_flag*)malloc(sizeof(t_flag));
	ls->flag->l = 0;
	ls->flag->a = 0;
	ls->flag->t = 0;
	ls->flag->r = 0;
	ls->flag->R = 0;

	ls->sort[0] = ft_sort_ascii;
	ls->sort[1] = ft_sort_time;

	time_t timer;
	timer = time(NULL);
	char *curr_time_str = ctime(&timer) + 4;
	int day;
	ls->curr_time = ft_get_time(curr_time_str, &ls->curr_year, &day);

	ft_read_args(ls, argc, argv);

	return (0);
}
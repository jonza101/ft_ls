/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 15:17:42 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/07 23:19:28 by zjeyne-l         ###   ########.fr       */
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
	ls->elem = NULL;

	ft_read_args(ls, argc, argv);
	ft_read(ls);

	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:30:32 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/04/09 18:37:37 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


void		ft_split_free(char **split)
{
	int i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

long long	ft_get_time(char *time_str, int *o_year, int *add_day)
{
	char **split = ft_strsplit(time_str, ' ');
	char **clck_split = ft_strsplit(split[2], ':');

	int sec = ft_atoi(clck_split[2]);
	int min = ft_atoi(clck_split[1]);
	int hour = ft_atoi(clck_split[0]);

	int day = ft_atoi(split[1]);
	*add_day = 0;
	int year = ft_atoi(split[3]);
	int month = 1;
	if (!ft_strcmp(split[0], "Jan"))
	{
		month = 1;
		*add_day = 1;
	}
	else if (!ft_strcmp(split[0], "Feb"))
	{
		month = 2;
		*add_day = -2;
	}
	else if (!ft_strcmp(split[0], "Mar"))
	{
		month = 3;
		*add_day = 1;
	}
	else if (!ft_strcmp(split[0], "Apr"))
		month = 4;
	else if (!ft_strcmp(split[0], "May"))
	{
		month = 5;
		*add_day = 1;
	}
	else if (!ft_strcmp(split[0], "Jun"))
		month = 6;
	else if (!ft_strcmp(split[0], "Jul"))
	{
		month = 7;
		*add_day = 1;
	}
	else if (!ft_strcmp(split[0], "Aug"))
	{
		month = 8;
		*add_day = 1;
	}
	else if (!ft_strcmp(split[0], "Sep"))
		month = 9;
	else if (!ft_strcmp(split[0], "Oct"))
	{
		month = 10;
		*add_day = 1;
	}
	else if (!ft_strcmp(split[0], "Nov"))
		month = 11;
	else if (!ft_strcmp(split[0], "Dec"))
	{
		month = 12;
		*add_day = 1;
	}

	*o_year = year;
	long long o_time = sec + min * 60 + hour * 60 * 60 + day * 60 * 60 * 24 + month * 60 * 60 * 24 * (30 + *add_day); //year * 60 * 60 * 24 * (30 + add_day) * 365;

	ft_split_free(clck_split);
	ft_split_free(split);

	return (o_time);
}

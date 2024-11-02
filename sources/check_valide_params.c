/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_valide_params.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:03:49 by lzhang2           #+#    #+#             */
/*   Updated: 2024/11/02 18:59:06 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/philosophers.h"

void	check_param(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error: invalid argument\n");
				exit(1);
			}
			j++;
		}
		i++;
	}
}

void	validation_params(char **argv)
{
	if (ft_atoi(argv[1]) < 2 || ft_atoi(argv[1]) > PHILO_MAX)
	{
		printf("Error: invalid number of philosophers\n");
		exit(1);
	}
	if (ft_atoi(argv[2]) < (ft_atoi(argv[3]) + ft_atoi(argv[4])))
	{
		printf("Error: invalid time to die\n");
		exit(1);
	}
	if (ft_atoi(argv[3]) <= 0)
	{
		printf("Error: invalid time to eat\n");
		exit(1);
	}
	if (ft_atoi(argv[4]) <= 0)
	{
		printf("Error: invalid time to sleep\n");
		exit(1);
	}
	if (argv[5] && ft_atoi(argv[5]) <= 0)
	{
		printf("Error: invalid number of times each philosopher must eat\n");
		exit(1);
	}
}
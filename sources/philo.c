/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:08:37 by lzhang2           #+#    #+#             */
/*   Updated: 2024/11/02 19:56:14 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/philosophers.h"

void	init_time(t_prog *prog)
{
	struct timeval	tv;

	prog->start_time = 0;
	gettimeofday(&tv, NULL);
	prog->start_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

unsigned long	get_time(t_prog *prog)
{
	struct timeval	tv;
	unsigned long	current_time;

	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (current_time - prog->start_time);
}

void	print_status(t_prog *prog, t_philo *philo, const char *status)
{
	pthread_mutex_lock(philo->print_lock);
	printf("%ld %d %s\n", get_time(prog), philo->id, status);
	pthread_mutex_unlock(philo->print_lock);
}

void	*philosopher_routine(void *philo_arg)
{
	t_philo	*philo;
	t_prog	*prog;

	philo = (t_philo *)philo_arg;
	prog = philo->prog;
	while (1)
	{
		print_status(philo->prog, philo, "is thinking");
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(philo->r_fork);
		print_status(philo->prog, philo, "is eating");
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		print_status(philo->prog, philo, "is sleeping");
		usleep(philo->time_to_sleep * 1000);
	}
	return (NULL);
}

int	philosopher_dead(t_prog *prog, t_philo *philo)
{
	int	is_dead;

	pthread_mutex_lock(philo->meal_lock);
	is_dead = (get_time(prog) - philo->last_meal >= philo->time_to_die
			&& !philo->eating);
	pthread_mutex_unlock(philo->meal_lock);
	return (is_dead);
}

int	check_if_dead(t_prog *prog)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&prog->dead_lock);
	while (i < prog->num_philos)
	{
		if (philosopher_dead(prog, &prog->philos[i]))
		{
			print_status(prog, &prog->philos[i], "died");
			pthread_mutex_lock(&prog->dead_lock);
			prog->dead_flag = 1;
			pthread_mutex_unlock(&prog->dead_lock);
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(&prog->dead_lock);
	return (0);
}

int	check_if_all_ate(t_prog *prog)
{
	int	i;
	int	finished_eating;

	i = -1;
	finished_eating = 0;
	if (prog->philos[0].num_times_to_eat == -1)
		return (0);
	while (++i < prog->num_philos)
	{
		pthread_mutex_lock(prog->philos[i].meal_lock);
		if (prog->philos[i].meals_eaten >= prog->philos[i].num_times_to_eat)
			finished_eating++;
		pthread_mutex_unlock(prog->philos[i].meal_lock);
	}
	if (finished_eating == prog->num_philos)
	{
		pthread_mutex_lock(&prog->dead_lock);
		prog->dead_flag = 1;
		pthread_mutex_unlock(&prog->dead_lock);
		return (1);
	}
	return (0);
}

void	*monitor(void *prog_arg)
{
	t_prog	*prog;

	prog = (t_prog *)prog_arg;
	while (1)
	{
		if (check_if_dead(prog) == 1 || check_if_all_ate(prog) == 1)
			break ;
		usleep(100);
	}
	return (NULL);
}

void	error_message(char *message)
{
	printf("%s", message);
	exit(1);
}

void	init_philos(t_prog *prog, char  **argv)
{
	int	i;

	i = -1;
	init_time(prog);
	while (++i < prog->num_philos)
	{
		prog->philos[i].id = i + 1;
		prog->philos[i].time_to_die = ft_atoi(argv[2]);
		prog->philos[i].time_to_eat = ft_atoi(argv[3]);
		prog->philos[i].time_to_sleep = ft_atoi(argv[4]);
		if (argv[5])
			prog->philos[i].num_times_to_eat = ft_atoi(argv[5]);
		else
			prog->philos[i].num_times_to_eat = -1;
		prog->philos[i].dead = &prog->dead_flag;
		prog->philos[i].print_lock = &prog->print_lock;
		prog->philos[i].dead_lock = &prog->dead_lock;
		prog->philos[i].meal_lock = &prog->meal_lock;
		prog->philos[i].l_fork = &prog->forks[i];
		prog->philos[i].r_fork = &prog->forks[(i + 1) % prog->num_philos];
		pthread_mutex_init(&prog->forks[i], NULL);
		if (pthread_create(&prog->philos[i].thread, NULL,
				philosopher_routine, &prog->philos[i]) != 0)
			error_message("Error: pthread_create failed\n");
	}
}

void	init_prog(t_prog *prog, char **argv)
{
	prog->dead_flag = 0;
	pthread_mutex_init(&prog->dead_lock, NULL);
	pthread_mutex_init(&prog->meal_lock, NULL);
	pthread_mutex_init(&prog->print_lock, NULL);
	prog->num_philos = ft_atoi(argv[1]);
	prog->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* prog->num_philos);
	if (!prog->forks)
		error_message("Error: malloc failed for forks\n");
	prog->philos = (t_philo *)malloc(sizeof(t_philo) * prog->num_philos);
	if (!prog->philos)
		error_message("Error: malloc failed\n");
	init_philos(prog, argv);
}

void	free_program(t_prog *prog, int num_philos)
{
	int	i;

	i = -1;
	while (++i < num_philos)
	{
		pthread_join(prog->philos[i].thread, NULL);
		pthread_mutex_destroy(&prog->forks[i]);
	}
	free(prog->philos);
	free(prog->forks);
	pthread_mutex_destroy(&prog->dead_lock);
	pthread_mutex_destroy(&prog->meal_lock);
	pthread_mutex_destroy(&prog->print_lock);
}

int	main(int argc, char**argv)
{
	t_prog	prog;
	pthread_t	monitor_thread;

	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments\n");
		exit(1);
	}
	check_param(argc, argv);
	validation_params(argv);
	init_time(&prog);
	init_prog(&prog, argv);
	if (pthread_create(&monitor_thread, NULL, monitor, &prog) != 0)
		error_message("Error: pthread_create for monitor failed\n");
	pthread_detach(monitor_thread);
	free_program(&prog, ft_atoi(argv[1]));
	return (0);
}

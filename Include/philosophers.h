/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:00:17 by lzhang2           #+#    #+#             */
/*   Updated: 2024/11/02 19:42:42 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/*free malloc*/
# include <stdlib.h>
/*gettimeofday: 功能：获取当前时间。*/
# include <sys/time.h>
/*usleep: 功能：使当前线程睡眠指定的微秒数（1,000,000 微秒 = 1 秒）*/
# include <unistd.h>
/*memset: 功能：将一块内存区域设置为指定的值。*/
# include <string.h>
/*printf*/
# include <stdio.h>
/*thread_create ;于创建一个新的线程
thread_detach: 功能：分离一个线程，使线程终止时自动释放资源
pthread_join: 功能：等待一个线程终止
pthread_mutex_init: 功能：初始化一个互斥锁
pthread_mutex_destroy: 功能：销毁一个互斥锁。
pthread_mutex_lock: 功能：锁定一个互斥锁。
pthread_mutex_unlock: 功能：解锁一个互斥锁。*/
# include <pthread.h>

# define PHILO_MAX 200
# define NUMBER_OF_PHILOSOPHERS 5
# define TIME_TO_DIE 800
# define TIME_TO_EAT 200
# define TIME_TO_SLEEP 200
# define NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT 7

typedef struct s_prog	t_prog;
/*必须事先申明才能在philo里建立成员t_prog* prog*/
typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	unsigned long	last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				num_times_to_eat;
	int				*dead;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
	t_prog			*prog;
}					t_philo;

typedef struct s_prog
{
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
	int				num_philos;
	t_philo			*philos;
	unsigned long 	start_time;
}					t_prog;

/*util.c*/
int		ft_atoi(const char *str);

/*check_valide_params.c*/
void	check_param(int argc, char **argv);
void	validation_params(char **argv);

#endif
/*
这些pthread函数在成功执行时通常返回0。如果函数执行失败，它们会返回一个非零的错误码，表示发生了错误。
1. **pthread_create**:
	pthread_t *thread`: 指向线程标识符的指针。
	const pthread_attr_t *attr`: 线程属性，通常使用默认属性，可以传入`NULL`。
	void *(*start_routine) (void *)`: 线程启动例程，即线程函数,
	接受一个`void *`类型的参数，返回一个`void *`类型的值。
	void *arg`: 传递给线程启动例程的参数。
  
   	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
							void *(*start_routine) (void *), void *arg);

2. **pthread_detach**:
	pthread_t thread`: 线程标识符。
 
	int pthread_detach(pthread_t thread);

3. **pthread_join**:
	pthread_t thread`: 线程标识符。
	void **retval`: 指向线程返回值的指针，通常使用`NULL`。

	int pthread_join(pthread_t thread, void **retval);
  
4. **pthread_mutex_init**:
	pthread_mutex_t *mutex`: 指向互斥锁的指针。
	const pthread_mutexattr_t *attr`: 互斥锁属性，通常使用默认属性，可以传入`NULL`。

	int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

5. **pthread_mutex_destroy**:
	pthread_mutex_t *mutex`: 指向互斥锁的指针。

   int pthread_mutex_destroy(pthread_mutex_t *mutex);

6. **pthread_mutex_lock**:
	pthread_mutex_t *mutex`: 指向互斥锁的指针。

	int pthread_mutex_lock(pthread_mutex_t *mutex);
 
7. **pthread_mutex_unlock**:
	pthread_mutex_t *mutex`: 指向互斥锁的指针。
	
	int pthread_mutex_unlock(pthread_mutex_t *mutex);

在使用这些函数时，需要确保正确传递参数，并且在使用完互斥锁后及时销毁，以避免资源泄漏。*/
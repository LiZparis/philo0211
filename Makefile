NAME			= philo
NAME_BONUS		= philo_bonus

GREEN			= \033[0;32m
RED				= \033[0;31m
RESET			= \033[0m

CC     			= cc
CFLAGS 			= -Wall -Wextra -Werror -g3 -I ./Include

SRCS_DIR 		= ./sources/
BONUS_SRCS_DIR 	= ./bonus_sources/

SRCS		= $(addprefix $(SRCS_DIR),			\
					philo.c						\
					check_valide_params.c		\
					utils.c)					\
					# init.c					\
					# threads.c)

# BONUS_SRCS	= $(addprefix $(BONUS_SRCS_DIR),\
# 					philo_bonus.c				\
# 					utils_bonus.c				\
# 					init_bonus.c				\
# 					threads_bonus.c)

OBJS 			= $(SRCS:.c=.o)
OBJS_BONUS 		= $(BONUS_SRCS:.c=.o)

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
				@echo "$(GREEN)$(NAME) is compiled!$(RESET)"
				@echo

bonus:			$(OBJS_BONUS)
				$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)
				@echo "$(GREEN)$(NAME_BONUS) is compiled!$(RESET)"
				@echo

%.o:			%.c
				$(CC) $(CFLAGS) -c $< -o $@

REMOVE			= rm -f
clean:
				$(REMOVE) $(OBJS) $(OBJS_BONUS)
				@echo "$(RED)Object files are removed!$(RESET)"
				@echo

fclean:			clean
				$(REMOVE) $(NAME) $(NAME_BONUS)
				@echo "$(RED)$(NAME) and $(NAME_BONUS) are removed!$(RESET)"
				@echo

re:				fclean all

.PHONY:			all clean fclean re bonus
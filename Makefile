NAME = pipex
BONUS_NAME = pipex_bonus

SRCS_DIR = ./srcs
SRCS = pipex.c pipex_aux.c errors.c
OBJS_DIR = ./objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

BONUS_SRCS_DIR = ./srcs_bonus
BONUS_SRCS = bonus_pipex.c bonus_pipex_aux.c
BONUS_OBJS_DIR = ./objs_bonus
BONUS_OBJS = $(addprefix $(BONUS_OBJS_DIR)/, $(BONUS_SRCS:.c=.o))
$(BONUS_OBJS_DIR)/%.o: $(BONUS_SRCS_DIR)/%.c
	@mkdir -p $(BONUS_OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

CFLAGS = -Wall -Wextra -Werror
LIBFT_DIR = ./includes/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@cc $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)
	@echo $(NAME) ready!

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT) 
	@cc $(CFLAGS) -o $(BONUS_NAME) $(BONUS_OBJS) $(LIBFT)
	@echo $(BONUS_NAME) ready!

$(LIBFT):
	@echo libft compiling!
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory
	@echo libft compiled and ready!

clean:
	@rm -f -r $(OBJS_DIR) $(BONUS_OBJS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@echo objects clean

fclean: clean
	@rm -f $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@echo $(NAME) fclean

re: fclean all

test1:
	./pipex Makefile cat wc out && echo $? && cat out && rm out
test2:
	./pipex "      Makefile" cat wc out && echo $? && cat out && rm out
test3:
	./pipex " " cat wc out && echo $? && cat out && rm out
test4:
	./pipex "" cat wc out && echo $? && cat out && rm out
test5:
	./pipex asdasdasd cat wc out && echo $? && cat out && rm out


test6:
	./pipex Makefile "    cat" wc out && echo $? && cat out && rm out
test7:
	./pipex Makefile " " wc out && echo $? && cat out && rm out
test8:
	./pipex Makefile "" wc out && echo $? && cat out && rm out
test9:
	./pipex Makefile "asdasdasd" wc out && echo $? && cat out && rm out
test10:
	./pipex Makefile "asdasdasd asdasdasd" wc out && echo $? && cat out && rm out
test11:
	./pipex Makefile "asdasdasd -asdasd" wc out && echo $? && cat out && rm out
test12:
	./pipex Makefile "cat -asdasdasd" wc out && echo $? && cat out && rm out
test13:
	./pipex Makefile "cat asdasdasd" wc out && echo $? && cat out && rm out


test14:
	./pipex Makefile cat "     wc" out && echo $? && cat out && rm out
test15:
	./pipex Makefile cat " " out && echo $? && cat out && rm out
test16:
	./pipex Makefile cat "" out && echo $? && cat out && rm out
test17:
	./pipex Makefile cat "asdasdasd" out && echo $? && cat out && rm out
test18:
	./pipex Makefile cat "asdasdasdasd asdasdasd" out && echo $? && cat out && rm out
test19:
	./pipex Makefile cat "asdasdasdasd -asdasdasd" out && echo $? && cat out && rm out
test20:
	./pipex Makefile cat "wc -adsasdasdasd" out && echo $? && cat out && rm out
test21:
	./pipex Makefile cat "wc asdasdasdasdasd" out && echo $? && cat out && rm out


test22:
	./pipex "  " cat " " out && echo $? && cat out && rm out
test23:
	./pipex "   " "   " wc out && echo $? && cat out && rm out
test24:
	./pipex Makefile "  " "   " out && echo $? && cat out && rm out

test25:
	./pipex "" cat "" out && echo $? && cat out && rm out
test26:
	./pipex "" "" wc out && echo $? && cat out && rm out
test27:
	./pipex Makefile "" "" out && echo $? && cat out && rm out

test28:
	./pipex asdasdasd cat qweqweqwe out && echo $? && cat out && rm out
test29:
	./pipex asdasdasd qweqweqwe wc out && echo $? && cat out && rm out
test30:
	./pipex Makefile asdasdasd qweqweqweqwe out && echo $? && cat out && rm out


test31:
	./pipex "" "" "" out && echo $? && cat out && rm out
test32:
	./pipex zxczxczxczxc asdasdasd qweqweqweqwe out && echo $? && cat out && rm out

.PHONY: all bonus fclean clean re
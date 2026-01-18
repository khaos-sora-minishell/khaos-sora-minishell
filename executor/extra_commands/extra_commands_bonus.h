/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_commands_bonus.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:19:00 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 20:19:01 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTRA_COMMANDS_BONUS_H
# define EXTRA_COMMANDS_BONUS_H

# include <termios.h>

# define MATRIX_WIDTH 80
# define MATRIX_HEIGHT 24

# define C_RESET "\033[0m"
# define C_RED "\033[31m"
# define C_GREEN "\033[32m"
# define C_YELLOW "\033[33m"
# define C_BLUE "\033[34m"
# define C_MAGENTA "\033[35m"
# define C_CYAN "\033[36m"
# define C_WHITE "\033[37m"

# define BG_RED "\033[41m"
# define BG_GREEN "\033[42m"
# define BG_YELLOW "\033[43m"
# define BG_BLUE "\033[44m"
# define BG_MAGENTA "\033[45m"
# define BG_CYAN "\033[46m"
# define BG_BLACK "\033[40m"

# define CLEAR "\033[2J"
# define HOME "\033[H"

# define KEY_CTRL_C 3
# define KEY_CTRL_D 4
# define KEY_ESC 27
# define KEY_SPACE 32

# define KEY_ARROW_UP 'A'
# define KEY_ARROW_DOWN 'B'
# define KEY_ARROW_RIGHT 'C'
# define KEY_ARROW_LEFT 'D'

# define KEY_QUIT 'q'
# define KEY_ROTATE KEY_ARROW_UP
# define KEY_SOFT_DROP KEY_ARROW_DOWN
# define KEY_RIGHT KEY_ARROW_RIGHT
# define KEY_LEFT KEY_ARROW_LEFT
# define KEY_HARD_DROP KEY_SPACE

# include "minishell.h"

typedef void		(*t_extra_command_func)(char **args, t_shell *shell);
typedef const int	(*t_tetromino_shapes)[4][4][4];

typedef struct s_extra_command
{
	char					*name;
	t_extra_command_func	func;
}	t_extra_command;

typedef struct s_tetris
{
	const	int		(*pieces)[4][4][4];
	int				**board;
	int				board_w;
	int				board_h;
	int				term_w;
	int				term_h;
	int				offset_x;
	int				offset_y;
	int				score;
	int				level;
	int				lines;
	int				current_piece;
	int				rotation;
	int				pos_x;
	int				pos_y;
	int				tick;
	int				speed;
	unsigned int	random_number_seed;
	int				running;
}					t_tetris;

int					is_extra_command(char *cmd);
void				execute_extra_command(char **args, t_shell *shell);
void				harici_matrix(char **args, t_shell *shell);
void				akivam_executer(char **args, t_shell *shell);
void				set_terminal_name(char **args, t_shell *shell);
unsigned int		simple_rand(unsigned int *seed);
void				simple_delay(void);
void				write_at(int row, int col, char c, int bright);
void				draw_rain(int *drops, int width, int height,
						unsigned int *s);
void				set_prompt(char **args, t_shell *shell);
void				set_background(char **args, t_shell *shell);
void				start_game(char **args, t_shell *shell);
void				init_tetris(t_tetris *t, void *arena);
void				setup_tetris_terminal(struct termios *old);
void				handle_input(t_shell *shell);
void				update_game(t_shell *shell);
void				render(t_tetris *t);
void				rotate_piece(t_tetris *t);
void				move_left(t_tetris *t);
void				move_right(t_tetris *t);
void				soft_drop(t_tetris *t);
void				hard_drop(t_tetris *t);
int					check_collision(t_tetris *t, int new_x, int new_y,
						int new_rot);
void				lock_piece(t_tetris *t);
void				spawn_piece(t_tetris *t);
void				clear_lines(t_tetris *t);
int					calc_speed(int level);

t_tetromino_shapes	get_tetromino_shapes(void);

#endif

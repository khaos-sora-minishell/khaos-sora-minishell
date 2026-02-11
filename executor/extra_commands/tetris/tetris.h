/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetris.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:05:38 by harici            #+#    #+#             */
/*   Updated: 2026/02/12 02:00:53 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TETRIS_H
# define TETRIS_H

# include "minishell.h"
# include "garbage_collector.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <termios.h>
# include <string.h>
# include <fcntl.h>

# define KEY_ROTATE 'w'
# define KEY_LEFT 'a'
# define KEY_RIGHT 'd'
# define KEY_SOFT_DROP 's'
# define KEY_HARD_DROP ' '
# define KEY_QUIT 'q'
# define KEY_ESC 27
# define KEY_CTRL_C 3
# define KEY_CTRL_D 4

# define HOME "\033[H"
# define CLEAR "\033[2J"
# define TETRIS_BUFFER_SIZE 4096

typedef int	t_shape[4][4][4];

typedef struct s_tetris
{
	int				**board;
	int				board_w;
	int				board_h;
	const t_shape	*pieces;
	long long		score;
	int				level;
	int				lines;
	int				tick;
	int				speed;
	unsigned int	random_number_seed;
	int				running;
	int				pos_x;
	int				pos_y;
	int				rotation;
	int				current_piece;
	int				next_piece;
	int				paused;
	int				menu_selection;
	int				game_over;
	int				ghost_y;
}	t_tetris;

void			init_tetris(t_tetris *t, void *arena);
int				calc_speed(int level);
void			clear_lines(t_tetris *t);
int				check_collision(t_tetris *t, int new_x, int new_y, int rot);
void			lock_piece(t_tetris *t);
void			render(t_tetris *t);
void			spawn_piece(t_tetris *t);
void			rotate_piece(t_tetris *t);
void			move_left(t_tetris *t);
void			move_right(t_tetris *t);
void			soft_drop(t_tetris *t);
void			hard_drop(t_tetris *t);
const t_shape	*get_tetromino_shapes(void);
void			setup_tetris_terminal(struct termios *old);
void			simple_delay(void);
unsigned int	xorshift32(unsigned int *state);
unsigned int	get_random_seed(void);
void			append_string(char *buffer, int *pos, const char *str);
void			append_int(char *buffer, int *pos, int n);
void			render_game_over(t_tetris *t);
int				get_ghost_y(t_tetris *t);
void			render_board_line(t_tetris *t, char *buffer, int *pos, int y);
void			start_game(char **args, t_shell *shell);
void			handle_input(t_shell *shell);

#endif

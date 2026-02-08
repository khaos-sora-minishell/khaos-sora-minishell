/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_tetris.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:06:56 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 17:58:48 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

void	setup_tetris_terminal(struct termios *old)
{
	struct termios	new;

	tcgetattr(STDIN_FILENO, old);
	new = *old;
	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VMIN] = 0;
	new.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &new);
	write(1, "\033[?25l", 6);
}

void	simple_delay(void)
{
	volatile long long	i;

	i = 0;
	while (i < 35000000)
		i++;
}

unsigned int	get_random_seed(void)
{
	unsigned int	seed;
	int				fd;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (42);
	read(fd, &seed, sizeof(seed));
	close(fd);
	return (seed);
}

unsigned int	xorshift32(unsigned int *state)
{
	unsigned int	x;

	x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*state = x;
	return (x);
}

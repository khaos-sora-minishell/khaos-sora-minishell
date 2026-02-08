/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_print_stats.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:04:56 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:04:56 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <unistd.h>

static void	gc_write_str(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(1, str, i);
}

/*convert size_t to string using stack buffer (no allocation)*/
static void	gc_write_number(size_t value)
{
	char	buffer[32];
	int		len;
	size_t	temp;

	len = 0;
	temp = value;
	if (temp == 0)
		buffer[len++] = '0';
	while (temp > 0 && len < 31)
	{
		buffer[len++] = (temp % 10) + '0';
		temp /= 10;
	}
	while (--len >= 0)
		write(1, &buffer[len], 1);
}

static void	gc_print_line(const char *label, size_t value)
{
	gc_write_str("  ");
	gc_write_str(label);
	gc_write_str(": ");
	gc_write_number(value);
	gc_write_str(" bytes\n");
}

static void	gc_print_count(const char *label, size_t value)
{
	gc_write_str("  ");
	gc_write_str(label);
	gc_write_str(": ");
	gc_write_number(value);
	gc_write_str("\n");
}

/*
**	print gc statistics to stdout
**	shows allocation/free counts and memory usage
**	NOTE: uses stack buffer to avoid recursive allocation
*/
void	gc_print_stats(t_gc_context *contex)
{
	if (!contex)
		return ;
	gc_write_str("=== GC Statistics ===\n");
	gc_print_line("Total Allocated", contex->total_allocated);
	gc_print_line("Total Freed", contex->total_freed);
	gc_print_line("Current Usage", contex->current_usage);
	gc_print_line("Peak Usage", contex->peak_usage);
	gc_print_count("Allocation Count", contex->allocation_count);
	gc_print_count("Free Count", contex->free_count);
	gc_print_count("Scope Depth", contex->scope_depth);
	gc_write_str("=====================\n");
}

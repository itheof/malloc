/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brutalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 18:05:32 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/07 20:14:45 by tvallee          ###   ########.fr       */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/malloc.h"
#include "../libft/include/libft.h"

void	puthex(unsigned char byte)
{
	char	map[] = "0123456789ABCDEF";

	ft_putchar(map[byte >> 4]);
	ft_putchar(map[byte & 0xF]);
}

size_t	my_align_size(size_t size, size_t alignment)
{
	size_t	truncated;

	truncated = size & (~(alignment - 1));
	if (truncated < size)
	{
		return (truncated + alignment);
	}
	else
		return (truncated);
}

size_t	my_block_size(size_t size)
{
	size_t	aligned;

	aligned = my_align_size(size + sizeof(t_block) * 2, 0x8);
	if (aligned < (BLOCK_MIN_SIZE))
		return (BLOCK_MIN_SIZE);
	else
		return (aligned);
}

void	hexdump(void *ptr)
{
	size_t	l_count;
	t_block_free	*current;

	l_count = 0;
	current = (t_block_free*)((t_block*)ptr - 1);
	for (size_t n = 0; n < BLOCK_SIZE(current->header.size); n++)
	{
		if (l_count == 10)
		{
			ft_putchar('\n');
			l_count = 0;
		}
		puthex((((unsigned char*)current)[n]));
		ft_putchar(' ');
		l_count++;
	}
	ft_putchar('\n');
}

int main()
{
	void		*table[200] = {NULL};
	void		**ptr;
	uint32_t	rand;
	unsigned	type;

	write(1, "=========BEGIN BRUTALIZE=========\n", sizeof("=========BEGIN BRUTALIZE========="));
	while (1)
	{
		ft_putendl("=> new iteration");
		type = 0;//arc4random_uniform(3);
		if (type == 0)
		{
			rand = arc4random_uniform(TINY_SIZE + 1);
		}
		else if (type == 1)
		{
			do {
				rand = arc4random_uniform(SMALL_SIZE + 1);
			} while (rand <= TINY_SIZE);
		}
		else
		{
			do {
				rand = arc4random_uniform(1024 * 1024 * 42);
			} while (rand <= SMALL_SIZE);
		}

		ptr = &(table[arc4random_uniform(200)]);

		if (*ptr == NULL)
		{
			*ptr = malloc(rand);
		}
		/*else if (arc4random_uniform(2))
			*ptr = realloc(*ptr, rand);*/
		else
		{
			hexdump(*ptr);
			free(*ptr);
			*ptr = NULL;
		}
		if (*ptr != NULL)
		{
			arc4random_buf(*ptr, rand);
		}
	}
}

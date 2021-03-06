/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_resize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 15:41:18 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/18 14:22:49 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

t_block			*block_shrink(t_block *block, size_t size, unsigned type)
{
	t_block	*next;
	t_block	old;
	size_t	extra_space;

	extra_space = BLOCK_SIZE(block->size) - size;
	if (allocs_assert_available_block_type(extra_space, type))
	{
		old = *block;
		block->size = size;
		block->flags.bound_left = old.flags.bound_left;
		block_update_footer(block);
		next = get_next_block(block);
		next->size = extra_space;
		next->flags.bound_right = old.flags.bound_right;
		block_update_footer(next);
		free(next + 1);
	}
	return (block);
}

t_block			*block_enlarge(t_block *block, size_t diff, unsigned type)
{
	t_block_free	*next;
	t_block			*reserved;

	if ((next = (t_block_free*)get_next_block(block)) == NULL)
		return (NULL);
	if (next->header.flags.available == FALSE ||
			BLOCK_SIZE(next->header.size) <= diff)
		return (NULL);
	reserved = block_create(next, diff, type);
	block->size += reserved->size;
	block_update_footer(block);
	return (block);
}

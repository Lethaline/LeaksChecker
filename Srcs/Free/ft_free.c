/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lolemmen <lolemmen@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:38:18 by lolemmen          #+#    #+#             */
/*   Updated: 2022/11/28 16:03:24 by lolemmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/leaks.h"

static void	(*g_free)(void *) = 0;

static void	ft_free_init(void)
{
	g_free = (void (*)(void *))dlsym(RTLD_NEXT, "free");
	if (g_free == 0)
		fprintf(stderr, "Error in dlsym: %s\n", dlerror());
}

static void	ft_remove_address(long address)
{
	int	i;

	i = 0;
	while (i < LEN_MAX)
	{
		if (g_allocationtab[i].address == address)
		{
			g_allocationtab[i].address = 0;
			g_allocationtab[i].size = 0;
			g_allocationtab[i].line[0] = '\0';
			return ;
		}
		i++;
	}
	fprintf(stderr, "Trying to free memory that have already freed or"
		"haven't ever allocated\n"
		"Address 0x%lx not found", address);
	exit(2);
}

void	free(void *p)
{
	if (g_free == 0)
		ft_free_init();
	ft_remove_address((long) p);
	g_free(p);
	g_allocations--;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lolemmen <lolemmen@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:40:31 by lolemmen          #+#    #+#             */
/*   Updated: 2022/11/28 15:48:18 by lolemmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/leaks.h"

static void	*(*g_malloc)(unsigned long) = 0;

static void	ft_malloc_init(void)
{
	g_malloc = (void *(*)(unsigned long))dlsym(RTLD_NEXT, "malloc");
	if (g_malloc == 0)
		fprintf(stderr, "Error in dlsym: %s\n", dlerror());
	ft_memset(&g_allocationtab, 0, sizeof(g_allocationtab));
}

static void	ft_stackcpy(char *dst, char **tab, int size)
{
	int		total;
	int		i;
	int		len;
	char	*function;

	total = 0;
	i = 0;
	while (i < size)
	{
		function = ft_split3(tab[i]);
		len = ft_strlen(function);
		total += len;
		if (total + 2 > LEN_MAX)
			return ;
		if (total != len)
		{
			dst[0] = '\n';
			dst += 1;
		}
		ft_strncpy(dst, function, len + 1);
		dst += len;
		i++;
	}
}

t_call	*ft_find_next(void)
{
	int	i;

	i = 0;
	while (i < LEN_MAX)
	{
		if (g_allocationtab[i].address == 0)
			return (&(g_allocationtab[i]));
		i++;
	}
	return (0);
}

static void	ft_add_new_allocation(t_call *allocation)
{
	t_call	*position;

	position = ft_find_next();
	if (position == 0)
	{
		fprintf(stderr, "Not enought space\n");
		exit(1);
	}
	ft_memcpy(position, allocation, sizeof(t_call));
}

void	*malloc(unsigned long size)
{
	t_call	call;
	void	*p;
	void	*stack[128];
	char	**strs;
	int		frames;

	if (g_malloc == 0)
		ft_malloc_init();
	p = g_malloc(size);
	call.address = (long)p;
	call.size = size;
	call.line[0] = '\0';
	frames = backtrace(stack, 128);
	strs = backtrace_symbols(stack, frames);
	ft_stackcpy(call.line, strs, frames);
	ft_add_new_allocation(&call);
	g_allocations++;
	return (p);
}

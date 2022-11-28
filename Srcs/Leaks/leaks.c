/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lolemmen <lolemmen@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 11:04:25 by lolemmen          #+#    #+#             */
/*   Updated: 2022/11/28 14:20:28 by lolemmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/leaks.h"

int nbAllocation = 0;
t_call allocationTab[size_max];

static void	ft_print_call(t_call *call)
{
	fprintf(stderr, "Leak at address: 0x%lx | Leaked %ld bytes\n%s\n\n"
		, call->address, call->size, call->line);
}

static void	ft_print_allocation(void)
{
	int	i;

	i = 0;
	while (i < size_max)
	{
		if (allocationTab[i].address != 0)
			ft_print_call(&(allocationTab[i]));
		i++;
	}
}

static int	ft_leaks_size(void)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (i < size_max)
	{
		if (allocationTab[i].address != 0)
			total += allocationTab[i].size;
		i++;
	}
	return (total);
}

void	ft_check_leaks()
{
	int	total;

	if (nbAllocation != 0)
	{
		total = ft_leaks_size();
		fprintf(stderr, "\nLEAKS: there are %d leaks. Total size: %d bytes\n\n"
		, nbAllocation, total);
		ft_print_allocation();
	}
}

static void	(*ft_free)(void *) = 0;



static void	ft_remove_address(long address)
{
	int	i;

	i = 0;
	while (i < size_max)
	{
		if (allocationTab[i].address == address)
		{
			allocationTab[i].address = 0;
			allocationTab[i].size = 0;
			allocationTab[i].line[0] = '\0';
			return ;
		}
		i++;
	}
	fprintf(stderr, "Trying to free memory that have already freed or"
		"haven't ever allocated\n"
		"Address 0x%lx not found", address);
	exit(2);
}

static void *(*ft_malloc)(unsigned long) = 0;

static void	ft_malloc_init(void)
{
	ft_malloc = (void *(*)(unsigned long))dlsym(RTLD_NEXT, "malloc");
	if (ft_malloc == 0)
		fprintf(stderr, "Error in dlsym: %s\n", dlerror());
	ft_memset(&allocationTab, 0, sizeof(allocationTab));
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
		if (total + 2 > size_max)
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

t_call	*ft_find_next()
{
	int	i;

	i = 0;
	while (i < size_max)
	{
		if (allocationTab[i].address == 0)
			return (&(allocationTab[i]));
		i++;
	}
	return (0);
}

static void	ft_add_new_allocation(t_call *allocation)
{
	t_call *position;

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

	if (ft_malloc == 0)
		ft_malloc_init();
	p = ft_malloc(size);
	call.address = (long)p;
	call.size = size;
	call.line[0] = '\0';
	frames = backtrace(stack, 128);
	strs = backtrace_symbols(stack, frames);
	ft_stackcpy(call.line, strs, frames);
	ft_add_new_allocation(&call);
	nbAllocation++;
	return (p);
}

static void	ft_free_init(void)
{
	ft_free = (void (*)(void *))dlsym(RTLD_NEXT, "free");
	if (ft_free == 0)
		fprintf(stderr, "Error in dlsym: %s\n", dlerror());
}

void	free(void *p)
{
	if (ft_free == 0)
		ft_free_init();

	ft_remove_address((long) p);
	ft_free(p);
	nbAllocation--;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lolemmen <lolemmen@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 11:04:25 by lolemmen          #+#    #+#             */
/*   Updated: 2022/11/28 15:48:13 by lolemmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/leaks.h"

int			g_allocations = 0;

static void	ft_print_call(t_call *call)
{
	fprintf(stderr, "Leak at address: 0x%lx | Leaked %ld bytes\n%s\n\n",
		call->address, call->size, call->line);
}

static void	ft_print_allocation(void)
{
	int	i;

	i = 0;
	while (i < LEN_MAX)
	{
		if (g_allocationtab[i].address != 0)
			ft_print_call(&(g_allocationtab[i]));
		i++;
	}
}

static int	ft_leaks_size(void)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (i < LEN_MAX)
	{
		if (g_allocationtab[i].address != 0)
			total += g_allocationtab[i].size;
		i++;
	}
	return (total);
}

void	ft_check_leaks(void)
{
	int	total;

	if (g_allocations != 0)
	{
		total = ft_leaks_size();
		fprintf(stderr, "\nLEAKS: there are %d leaks. Total size: %d bytes\n\n",
			g_allocations, total);
		ft_print_allocation();
	}
}

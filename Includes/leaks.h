/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lolemmen <lolemmen@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 11:04:28 by lolemmen          #+#    #+#             */
/*   Updated: 2022/11/28 14:05:42 by lolemmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEAKS_H
# define LEAKS_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <execinfo.h>
# include <dlfcn.h>

# define size_max 1024

typedef struct s_call
{
	long			address;
	unsigned long	size;
	char			line[size_max];
}	t_call;

void	ft_check_leaks(void);
void	free(void *p);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memset(void *str, int c, size_t len);
char	*ft_strncpy(char *dst, const char *src, size_t len);
char	*ft_split3(char *s);
size_t	ft_strlen(const char *str);

#endif

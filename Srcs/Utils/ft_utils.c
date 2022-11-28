/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lolemmen <lolemmen@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 12:23:29 by lolemmen          #+#    #+#             */
/*   Updated: 2022/11/28 14:09:05 by lolemmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/leaks.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*s1;

	i = 0;
	s1 = (char *)dst;
	if (n == 0 || dst == src)
		return (dst);
	while (i < n)
	{
		*(char *)s1 = *(char *)src;
		s1++;
		src++;
		i++;
	}
	return (dst);
}

void	*ft_memset(void *str, int c, size_t len)
{
	size_t	i;
	char	*s1;

	i = 0;
	s1 = (char *)str;
	while (i < len)
	{
		*(char *)s1 = (unsigned char)c;
		s1++;
		i++;
	}
	return (str);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	if (len < 0 || !dst || !src)
		return (NULL);
	while (src[i] && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_split3(char *s)
{
	int	len;
	int	spaces;
	int	i;

	len = ft_strlen(s);
	i = len - 1;
	spaces = 0;
	while (i >= 0)
	{
		if (s[i] == 32)
			spaces++;
		if (spaces == 3)
			return (&s[i]);
		i--;
	}
	return (s);
}

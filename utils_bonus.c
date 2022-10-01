/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 20:35:48 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 16:04:21 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex_bonus.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((*(s1 + i) || *(s2 + i)) && i < n)
	{
		if (*(s1 + i) != *(s2 + i))
			return (*(unsigned char *)(s1 + i) - *(unsigned char *)(s2 + i));
		i++;
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (*(unsigned char *)(s + len) != '\0')
		len++;
	return (len);
}

size_t	ft_strpplen(char **s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (*(unsigned char **)(s + len) != 0)
		len++;
	return (len);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	k;
	char	*str;

	k = 0;
	if (s1 == (void *)0 || s2 == (void *)0)
		return (NULL);
	str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (*(s1 + i) != '\0')
		*(str + k++) = *(s1 + i++);
	i = 0;
	while (*(s2 + i) != '\0')
		*(str + k++) = *(s2 + i++);
	*(str + k) = '\0';
	return (str);
}

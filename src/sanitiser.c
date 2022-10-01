/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitiser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 20:56:40 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 20:57:04 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex.h"

static char	**make_new_args(char **args, int index)
{
	t_runner	runner;
	char		**new_args;
	char		*str;

	runner.i = 0;
	runner.j = 0;
	runner.len = ft_strpplen(args);
	new_args = (char **)malloc(sizeof(char *) * runner.len);
	if (new_args == NULL)
		return (NULL);
	while (args[runner.j] != 0)
	{
		if (runner.i == index)
		{
			str = ft_strdup("\' \'");
			runner.j += 2;
		}
		else
			str = ft_strdup(args[runner.j++]);
		new_args[runner.i] = str;
		runner.i++;
	}
	ft_free_split(args);
	return (new_args);
}

char	**ft_cmd_sanitiser(char **cmd_args)
{
	char	**tmp;
	char	last;
	int		index;
	int		i;

	tmp = cmd_args;
	last = '\0';
	index = 0;
	i = 0;
	while (tmp[i] != 0)
	{
		if (ft_strlen(tmp[i]) == 1 && *tmp[i] == '\'')
		{
			if (last == *tmp[i])
				return (ft_cmd_sanitiser(make_new_args(cmd_args, index)));
			index++;
			last = *tmp[i];
		}
		i++;
	}
	return (cmd_args);
}

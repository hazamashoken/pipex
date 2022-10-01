/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 20:57:45 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 20:58:13 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex.h"

char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (ft_strncmp("PATH", *envp + i, 4))
		i++;
	return (*envp + i + 5);
}

char	*get_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	if (access(cmd, 0) == 0)
		return (cmd);
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, 0) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

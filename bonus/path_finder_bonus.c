/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:05:35 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 21:25:38 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex_bonus.h"

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

char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (ft_strncmp("PATH", *envp + i, 4))
		i++;
	return (*envp + i + 5);
}

void	path_finder(t_ppxb *pipex, char **envp)
{
	pipex->env_path = find_path(envp);
	pipex->cmd_paths = ft_split(pipex->env_path, ':');
	if (!pipex->cmd_paths)
		pipe_free(pipex);
}

void	cmd_counter(int argc, t_ppxb *pipex)
{
	pipex->cmd_nmbs = argc - 3 - pipex->here_doc;
	pipex->pid = (int *)malloc(sizeof(int) * pipex->cmd_nmbs);
	pipex->pipe_nmbs = 2 * (pipex->cmd_nmbs - 1);
	pipex->pipe = (int *)malloc(sizeof(int) * pipex->pipe_nmbs);
	if (!pipex->pipe)
		perr_msg("pipe", EXIT_FAILURE);
}

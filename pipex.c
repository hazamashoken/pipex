/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 03:11:10 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 11:29:51 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex.h"

int	err_msg(char *str)
{
	write(2, str, ft_strlen(str));
	exit(EXIT_FAILURE);
}

int	perr_msg(char *str)
{
	if (errno == 0)
		write(2, "Error\n", 6);
	else
		perror(str);
	exit(EXIT_FAILURE);
}

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

char	**make_new_args(char **args, int index)
{
	char	**tmp;
	int		len;
	char	**new_args;
	int		i;
	int		j;

	tmp = args;
	len = 0;
	j = 0;
	i = 0;
	while (tmp[len] != 0)
		len++;
	new_args = (char **)malloc(sizeof(char *) * len);
	if (new_args == NULL)
		return (NULL);
	while (args[j] != 0)
	{
		if (i == index)
		{
			new_args[i] = ft_strdup("\' \'");
			j++;
		}
		else
			new_args[i] = ft_strdup(args[j]);
		i++;
		j++;
	}
	ft_free_split(args);
	i = 0;
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

void	free_parent(t_pipex *pipex)
{
	int	i;

	i = 0;
	close(pipex->infile);
	close(pipex->outfile);
	while (pipex->cmd_paths[i])
	{
		free(pipex->cmd_paths[i]);
		i++;
	}
	free(pipex->cmd_paths);
}

void	free_child(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (pipex->cmd_args[i])
	{
		free(pipex->cmd_args[i]);
		i++;
	}
	free(pipex->cmd_args);
	free(pipex->cmd);
}

void	first_child(t_pipex pipex, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		perr_msg("fork");
	if (pid == 0)
	{
		pipex.infile = open(argv[1], O_RDONLY);
		if (pipex.infile < 0)
			perr_msg(argv[1]);
		close(pipex.tube[0]);
		dup2(pipex.tube[1], STDOUT_FILENO);
		dup2(pipex.infile, STDIN_FILENO);
		pipex.cmd_args = ft_split(argv[2], ' ');
		pipex.cmd = get_cmd(pipex.cmd_paths, pipex.cmd_args[0]);
		if (!pipex.cmd)
		{
			free_child(&pipex);
			err_msg("Command not found\n");
			exit(EXIT_FAILURE);
		}
		execve(pipex.cmd, pipex.cmd_args, envp);
		free_child(&pipex);
	}
}

void	second_child(t_pipex pipex, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		perr_msg("fork");
	if (pid == 0)
	{
		pipex.outfile = open(argv[4], O_TRUNC | O_CREAT | O_RDWR, 0000644);
		if (pipex.outfile < 0)
			perr_msg(argv[4]);
		close(pipex.tube[1]);
		dup2(pipex.tube[0], STDIN_FILENO);
		dup2(pipex.outfile, STDOUT_FILENO);
		pipex.cmd_args = ft_split(argv[3], ' ');
		pipex.cmd_args = ft_cmd_sanitiser(pipex.cmd_args);
		pipex.cmd = get_cmd(pipex.cmd_paths, pipex.cmd_args[0]);
		if (!pipex.cmd)
		{
			free_child(&pipex);
			err_msg("Command not found\n");
			exit(EXIT_FAILURE);
		}
		execve(pipex.cmd, pipex.cmd_args, envp);
		free_child(&pipex);
	}
}

void	close_pipes(t_pipex *pipex)
{
	close(pipex->tube[0]);
	close(pipex->tube[1]);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	errno = 0;
	if (argc != 5)
		return (err_msg("Invalid number of arguments.\n"));
	if (pipe(pipex.tube) < 0)
		return (perr_msg("Pipe"));
	pipex.paths = find_path(envp);
	pipex.cmd_paths = ft_split(pipex.paths, ':');
	first_child(pipex, argv, envp);
	second_child(pipex, argv, envp);
	close_pipes(&pipex);
	waitpid(-1, NULL, 0);
	waitpid(-1, NULL, 0);
	free_parent(&pipex);
	return (0);
}

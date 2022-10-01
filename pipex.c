/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 03:11:10 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 19:30:11 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex.h"

int	err_msg(char *str)
{
	write(2, str, ft_strlen(str));
	exit(EXIT_FAILURE);
}

int	perr_msg(char *str, int status)
{
	if (errno == 0)
		write(2, "Error\n", 6);
	else
		perror(str);
	exit(status);
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
	pipex.infile = open(argv[1], O_RDONLY);
	if (pipex.infile < 0)
		perr_msg(argv[1], 0);
	close(pipex.tube[0]);
	dup2(pipex.tube[1], STDOUT_FILENO);
	dup2(pipex.infile, STDIN_FILENO);
	pipex.cmd_args = ft_split(argv[2], ' ');
	pipex.cmd_args = ft_cmd_sanitiser(pipex.cmd_args);
	pipex.cmd = get_cmd(pipex.cmd_paths, pipex.cmd_args[0]);
	if (!pipex.cmd)
	{
		free_child(&pipex);
		err_msg("Command not found\n");
		exit(127);
	}
	if (execve(pipex.cmd, pipex.cmd_args, envp))
	{
		free_child(&pipex);
		perr_msg("execve", 1);
	}
	free_child(&pipex);
}

void	second_child(t_pipex pipex, char **argv, char **envp)
{
	pipex.outfile = open(argv[4], O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (pipex.outfile < 0)
		perr_msg(argv[4], 1);
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
		exit(127);
	}
	if (execve(pipex.cmd, pipex.cmd_args, envp))
	{
		free_child(&pipex);
		perr_msg("execve", 1);
	}
	free_child(&pipex);
}

void	close_pipes(t_pipex *pipex)
{
	close(pipex->tube[0]);
	close(pipex->tube[1]);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		return (err_msg("Invalid number of arguments.\n"));
	if (pipe(pipex.tube) < 0)
		return (perr_msg("Pipe", 1));
	pipex.paths = find_path(envp);
	pipex.cmd_paths = ft_split(pipex.paths, ':');
	pipex.pid_in = fork();
	if (pipex.pid_in < 0)
		perr_msg("fork", 1);
	if (pipex.pid_in == 0)
		first_child(pipex, argv, envp);
	pipex.pid_out = fork();
	if (pipex.pid_out < 0)
		perr_msg("fork", 1);
	if (pipex.pid_out == 0)
		second_child(pipex, argv, envp);
	close_pipes(&pipex);
	waitpid(pipex.pid_in, &pipex.status, 0);
	waitpid(pipex.pid_out, &pipex.status, 0);
	free_parent(&pipex);
	if (WEXITSTATUS(pipex.status) != 0)
		exit(WEXITSTATUS(pipex.status));
	exit(0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 13:49:28 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 03:29:29 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex_bonus.h"

int	err_msg(char *str)
{
	write(2, str, ft_strlen(str));
	exit (EXIT_FAILURE);
}

int	perr_msg(char *str)
{
	if (errno == 0)
		write(2, "Error\n", 6);
	else
		perror(str);
	exit(EXIT_FAILURE);
}

int	args_in(char *arg, t_ppxb *pipex)
{
	if (arg && !ft_strncmp("here_doc", arg, 9))
	{
		pipex->here_doc = 1;
		return (6);
	}
	else
	{
		pipex->here_doc = 0;
		return (5);
	}
}

void	here_doc(char *argv, t_ppxb *pipex)
{
	int		file;
	char	*buf;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0000644);
	if (file < 0)
		perr_msg("heredoc");
	while (1)
	{
		write(1, "heredoc> ", 9);
		buf = get_next_line(0);
		dprintf(2, "argv:%s buf:%s\n", argv, buf);
		if (ft_strncmp(argv, buf, ft_strlen(argv)) == 0)
			break ;
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		free(buf);
	}
	free(buf);
	close(file);
	pipex->infile = open(".heredoc_tmp", O_RDONLY);
	if (pipex->infile < 0)
	{
		unlink(".heredoc_tmp");
		perr_msg("heredoc");
	}
}

void	get_infile(char **argv, t_ppxb *pipex)
{
	if (!ft_strncmp("here_doc", argv[1], 9))
		here_doc(argv[2], pipex);
	else
	{
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile < 0)
			perr_msg(argv[1]);
	}
}

void	get_outfile(char **argv, t_ppxb *pipex, int argc)
{
	if (pipex->here_doc)
		pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0000644);
	else
		pipex->outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0000644);
	if (pipex->outfile < 0)
		perr_msg(argv[argc - 1]);
}

char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (ft_strncmp("PATH", *envp + i, 4))
		i++;
	return (*envp + i + 5);
}

void	parent_free(t_ppxb *pipex)
{
	int	i;

	i = 0;
	close(pipex->infile);
	close(pipex->outfile);
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
	while (pipex->cmd_paths[i])
	{
		free(pipex->cmd_paths[i]);
		i++;
	}
	free(pipex->cmd_paths);
	free(pipex->pipe);
}

void	child_free(t_ppxb *pipex)
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

void	pipe_free(t_ppxb *pipex)
{
	close(pipex->infile);
	close(pipex->outfile);
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
	free(pipex->pipe);
	err_msg("Environment");
	exit(1);
}

void	creat_pipes(t_ppxb *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_nmbs - 1)
	{
		if (pipe(pipex->pipe + 2 * i) < 0)
			parent_free(pipex);
		i++;
	}
}

void	msg_pipe(char *arg)
{
	write(2, "Command not found\n", ft_strlen("Command not found\n"));
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 1);
}

void	close_pipes(t_ppxb *pipex)
{
	int	i;

	i = 0;
	while (i < (pipex->pipe_nmbs))
	{
		close(pipex->pipe[i]);
		i++;
	}
}

void free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

static char	*get_cmd(char **paths, char *cmd)
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

static void	sub_dup2(int zero, int first)
{
	dup2(zero, 0);
	dup2(first, 1);
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
	free_split(args);
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

void	child(t_ppxb p, char **argv, char **envp, int argc)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		perr_msg("fork");
	else if (pid == 0)
	{
		if (p.idx == 0)
		{
			get_infile(argv, &p);
			sub_dup2(p.infile, p.pipe[1]);
		}
		else if (p.idx == p.cmd_nmbs - 1)
		{
			get_outfile(argv, &p, argc);
			sub_dup2(p.pipe[2 * p.idx - 2], p.outfile);
		}
		else
			sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);
		close_pipes(&p);
		p.cmd_args = ft_split(argv[2 + p.here_doc + p.idx], ' ');
		p.cmd_args = ft_cmd_sanitiser(p.cmd_args);
		p.cmd = get_cmd(p.cmd_paths, p.cmd_args[0]);
		if (!p.cmd)
		{
			msg_pipe(p.cmd_args[0]);
			child_free(&p);
			exit(EXIT_FAILURE);
		}
		execve(p.cmd, p.cmd_args, envp);
		child_free(&p);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_ppxb	pipex;

	errno = 0;
	if (argc < args_in(argv[1], &pipex))
		return (err_msg("Invalid number of arguments.\n"));
	pipex.cmd_nmbs = argc - 3 - pipex.here_doc;
	pipex.pipe_nmbs = 2 * (pipex.cmd_nmbs - 1);
	pipex.pipe = (int *)malloc(sizeof(int) * pipex.pipe_nmbs);
	if (!pipex.pipe)
		perr_msg("Pipe");
	pipex.env_path = find_path(envp);
	pipex.cmd_paths = ft_split(pipex.env_path, ':');
	if (!pipex.cmd_paths)
		pipe_free(&pipex);
	creat_pipes(&pipex);
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.cmd_nmbs)
		child(pipex, argv, envp, argc);
	close_pipes(&pipex);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	parent_free(&pipex);
	return (0);
}

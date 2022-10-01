/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 03:11:10 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 20:58:54 by tliangso         ###   ########.fr       */
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
	execve(pipex.cmd, pipex.cmd_args, envp);
	free_child(&pipex);
	perr_msg("execve", 1);
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
	execve(pipex.cmd, pipex.cmd_args, envp);
	free_child(&pipex);
	perr_msg("execve", 1);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		return (err_msg("Invalid number of arguments.\n"));
	if (pipe(pipex.tube) < 0)
		return (perr_msg("pipe", EXIT_FAILURE));
	pipex.paths = find_path(envp);
	pipex.cmd_paths = ft_split(pipex.paths, ':');
	pipex.pid_in = fork();
	if (pipex.pid_in < 0)
		perr_msg("fork", EXIT_FAILURE);
	if (pipex.pid_in == 0)
		first_child(pipex, argv, envp);
	pipex.pid_out = fork();
	if (pipex.pid_out < 0)
		perr_msg("fork", EXIT_FAILURE);
	if (pipex.pid_out == 0)
		second_child(pipex, argv, envp);
	close_pipes(&pipex);
	waitpid(pipex.pid_in, &pipex.status, 0);
	waitpid(pipex.pid_out, &pipex.status, 0);
	free_parent(&pipex);
	if (WEXITSTATUS(pipex.status) != 0)
		exit(WEXITSTATUS(pipex.status));
	exit(EXIT_SUCCESS);
}

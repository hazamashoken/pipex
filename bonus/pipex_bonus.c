/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 13:49:28 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/05 10:51:44 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex_bonus.h"

void	get_infile(char **argv, t_ppxb *pipex)
{
	if (!ft_strncmp("here_doc", argv[1], 9))
		here_doc(argv[2], pipex);
	else
	{
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile < 0)
			perr_msg(argv[1], EXIT_SUCCESS);
	}
}

void	get_outfile(char **argv, t_ppxb *p, int argc)
{
	if (p->here_doc)
		p->outfile = open(argv[argc - 1], 01 | O_CREAT | O_APPEND, 0000644);
	else
		p->outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0000644);
	if (p->outfile < 0)
		perr_msg(argv[argc - 1], EXIT_FAILURE);
}

void	child(t_ppxb p, char **argv, char **envp, int argc)
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
		exit(127);
	}
	execve(p.cmd, p.cmd_args, envp);
	child_free(&p);
	perr_msg("execve", EXIT_FAILURE);
}

int	main(int argc, char **argv, char **envp)
{
	t_ppxb	pipex;

	if (argc < args_in(argv[1], &pipex))
		return (err_msg("Invalid number of arguments.\n"));
	cmd_counter(argc, &pipex);
	path_finder(&pipex, envp);
	creat_pipes(&pipex);
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.cmd_nmbs)
	{
		pipex.pid[pipex.idx] = fork();
		if (pipex.pid[pipex.idx] < 0)
			perr_msg("fork", EXIT_FAILURE);
		else if (pipex.pid[pipex.idx] == 0)
			child(pipex, argv, envp, argc);
	}
	close_pipes(&pipex);
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.cmd_nmbs)
		waitpid(pipex.pid[pipex.idx], &pipex.status, 0);
	parent_free(&pipex);
	exit(EXIT_SUCCESS);
}

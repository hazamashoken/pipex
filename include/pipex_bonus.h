/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 13:51:38 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 21:27:46 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

//waitpid
# include	<sys/wait.h>

//open close
# include	<fcntl.h>

//dup2 execve fork pipe access
# include	<unistd.h>

//perror
# include	<stdio.h>

//exit
# include	<stdlib.h>

//errno
# include	<errno.h>

//get_next_line
# include	"get_next_line.h"

typedef struct s_ppxb
{
	pid_t	*pid;
	int		status;
	int		infile;
	int		outfile;
	char	*env_path;
	char	**cmd_paths;
	char	*cmd;
	char	**cmd_args;
	int		here_doc;
	int		cmd_nmbs;
	int		pipe_nmbs;
	int		*pipe;
	int		idx;
}t_ppxb;
typedef struct s_brunner
{
	int	i;
	int	j;
	int	len;
}	t_brunner;

//utils_bonus.c
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strpplen(char **s);

//utils2_bonus.c
char	*ft_strdup(char *s);
void	ft_free_split(char **str);

//split_bonus.c
char	**ft_split(char const *s, char c);

//path_finder_bonus.c
char	*find_path(char **envp);
char	*get_cmd(char **paths, char *cmd);
void	path_finder(t_ppxb *pipex, char **envp);
void	cmd_counter(int argc, t_ppxb *pipex);

//sanitiser_bonus.c
char	**ft_cmd_sanitiser(char **cmd_args);

//msg_handler_bonus.c
int		err_msg(char *str);
int		perr_msg(char *str, int status);
void	msg_pipe(char *arg);

//pipe_tools_bonus.c
void	pipe_free(t_ppxb *pipex);
void	creat_pipes(t_ppxb *pipex);
void	close_pipes(t_ppxb *pipex);
void	sub_dup2(int zero, int first);

//free_bonus.c
void	parent_free(t_ppxb *pipex);
void	child_free(t_ppxb *pipex);

//here_doc_bonus.c
int		args_in(char *arg, t_ppxb *pipex);
void	here_doc(char *argv, t_ppxb *pipex);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 20:22:06 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 15:42:24 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

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

typedef struct s_pipex
{
	pid_t	pid_in;
	pid_t	pid_out;
	int		tube[2];
	int		infile;
	int		outfile;
	char	*paths;
	char	**cmd_paths;
	char	**cmd_args;
	char	*cmd;

}	t_pipex;

typedef struct s_runner
{
	int	i;
	int	j;
	int	len;
}	t_runner;

//utils.c
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(char *s);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strpplen(char **s);
void	ft_free_split(char **str);

#endif

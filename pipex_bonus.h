/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 13:51:38 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 19:28:58 by tliangso         ###   ########.fr       */
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

//utils.c
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(char *s);
void	ft_free_split(char **str);
size_t	ft_strpplen(char **s);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:16:46 by tliangso          #+#    #+#             */
/*   Updated: 2022/10/01 21:18:58 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"pipex_bonus.h"

int	err_msg(char *str)
{
	write(2, str, ft_strlen(str));
	exit (EXIT_FAILURE);
}

int	perr_msg(char *str, int status)
{
	if (errno == 0)
		write(2, "Error\n", 6);
	else
		perror(str);
	exit(status);
}

void	msg_pipe(char *arg)
{
	write(2, "Command not found\n", ft_strlen("Command not found\n"));
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 1);
}

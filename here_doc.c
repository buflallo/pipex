/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 00:04:07 by hlachkar          #+#    #+#             */
/*   Updated: 2022/06/27 00:57:37 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "get_next_line.h"

size_t	ft_strlen1(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	write_to_fd1(char *lmt, int *fd)
{
	char	*str;

	str = get_next_line(0);
	while (str)
	{
		str[ft_strlen1(str) - 1] = '\0';
		if (ft_strcmp(str, lmt) == 0)
		{
			free(str);
			exit(1);
		}
		write(fd[1], str, ft_strlen1(str));
		write(fd[1], "\n", 1);
		free(str);
		str = get_next_line(0);
	}
	free(str);
}

void	here_doc(char *lmd)
{
	int		fd[2];
	int		pid;

	pipe(fd);
	pid = fork();
	if (pid)
	{
		close(fd[1]);
		dup2(fd[0], STDIN);
		waitpid(pid, NULL, 0);
	}
	else
	{
		write_to_fd1(lmd, fd);
	}
}

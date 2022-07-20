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

void	write_to_fd1(char *lmt, int fd[2], char *command, char **env)
{
	char	*str;

	str = get_next_line(0);
	dup2(fd[0],STDIN);
	while (str)
	{
		if (ft_strncmp(str, lmt, ft_strlen1(lmt)) == 0 || (lmt[0] == '\0' && str[0] == '\n'))
			break;
		write(fd[0], str, ft_strlen1(str));
		write(2, "\n", 1);
		free(str);
		str = get_next_line(0);
	}
	if (str)
		free(str);
	dup2(fd[1], STDOUT);
	execute(command, env);
}

void	here_doc(char *command, char **env, char *lmd)
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
		write_to_fd1(lmd, fd, command, env);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 08:07:00 by hlachkar          #+#    #+#             */
/*   Updated: 2022/06/27 00:56:21 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "get_next_line.h"

void	final_execute(char *command, char **env)
{
	int	pid;

	pid = fork();
	if (!pid)
		execute(command, env);
}

void	execution1(int ac, char **av, char **env)
{
	int	fileout;

	fileout = openfile(av[ac - 1], O_APPEND);
	dup2(fileout, STDOUT);
	here_doc(av[2]);
	redirection_pipe_line(av[3], env);
	execute(av[4], env);
	close(fileout);
}

void	execution2(int ac, char **av, char **env)
{
	int	filein;
	int	fileout;
	int	i;

	i = 2;
	fileout = openfile(av[ac - 1], OUTFILE);
	filein = openfile(av[1], INFILE);
	dup2(filein, STDIN);
	dup2(fileout, STDOUT);
	while (i < ac - 2)
		redirection_pipe_line(av[i++], env);
	final_execute(av[ac - 2], env);
	close(fileout);
}

int	main(int ac, char **av, char **env)
{
	int	status;

	status = 0;
	if (ft_strncmp(av[1], "here_doc", 8) == 0 && ac == 6)
		execution1(ac, av, env);
	else if (ac >= 5)
		execution2(ac, av, env);
	else
		write(STDERR, "Invalid number of arguments.\n", 29);
	while ((waitpid(-1, &status, 0)) > 0)
		;
	return (1);
}

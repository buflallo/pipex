/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 04:10:39 by hlachkar          #+#    #+#             */
/*   Updated: 2022/07/22 00:45:26 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

char	*str_join(char *s1, char *s2)
{
	char	*copy;
	int		i;
	int		j;

	copy = malloc(sizeof(char) * (ft_strchr(s1, 0) + ft_strchr(s2, 0) + 2));
	i = 0;
	j = 0;
	while (s1[j])
		copy[i++] = s1[j++];
	copy[i++] = '/';
	j = 0;
	while (s2[j])
		copy[i++] = s2[j++];
	copy[i] = 0;
	return (copy);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	while (--n > 0 && *s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s2 - *s1);
}

char	*ft_strndup(char *str, unsigned int n)
{
	char				*new;
	unsigned int		i;

	i = 0;
	new = malloc(sizeof(char) * (n + 1));
	while (i < n)
		new[i++] = *str++;
	new[n] = 0;
	return (new);
}

void	wrong_cmd(char *cmd)
{
	write(STDERR, "pipex: ", 7);
	write(STDERR, cmd, ft_strchr(cmd, 0));
	if (ft_strchr(cmd, '/') != -1 && access(cmd, F_OK) != 0)
		write(STDERR, ": No such file or directory\n", 28);
	else if (ft_strchr(cmd, '.') == 0)
		write(STDERR, ": file not executable\n", 23);
	else
		write(STDERR, ": command not found\n", 20);
	exit(127);
}

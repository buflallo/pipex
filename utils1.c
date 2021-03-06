/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 04:30:42 by hlachkar          #+#    #+#             */
/*   Updated: 2022/06/23 23:45:29 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	*ft_freedom(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
	str = NULL;
	return (0);
}

int	ft_words(char const *s, char c)
{
	int	i;
	int	cpt;
	int	j;

	i = 0;
	cpt = 0;
	j = 1;
	while (s[i])
	{
		if (s[i] == c)
			j = 1;
		else if (j == 1)
		{
			j = 0;
			cpt++;
		}
		i++;
	}
	return (cpt);
}

int	ft_charcount(const char *s, char c, int i)
{
	int	cnt;

	cnt = 0;
	while (s[i] && s[i] != c)
	{
		i++;
		cnt++;
	}
	return (cnt);
}

char	**ft_splitcpy(const char *s, char c, char **str, int wd)
{
	int	i;	
	int	j;
	int	cnt;

	i = 0;
	cnt = 0;
	while (s[i] && wd > cnt)
	{
		j = 0;
		while (s[i] == c)
			i++;
		str[cnt] = malloc(sizeof(char ) * ft_charcount(s, c, i) + 1);
		if (!str[cnt])
			return (ft_freedom(str));
		while (s[i] != c && s[i])
		{
			str[cnt][j] = s[i];
			i++;
			j++;
		}
		str[cnt][j] = '\0';
		cnt++;
	}
	str[cnt] = NULL;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	int		wd;
	char	**str;

	if (!s)
		return (0);
	wd = ft_words(s, c);
	str = malloc(sizeof(char *) * (wd + 1));
	if (str == NULL)
		return (0);
	return (ft_splitcpy(s, c, str, wd));
}

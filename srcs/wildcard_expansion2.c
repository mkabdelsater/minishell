/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expansion2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamaha <csamaha@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:14:02 by csamaha           #+#    #+#             */
/*   Updated: 2025/07/09 16:14:03 by csamaha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*process_quoted_segment(t_shell *shell, char *str,
	int *i, char quote)
{
	int		start;
	char	*temp;
	char	*expanded;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (!str[*i])
		return (NULL);
	temp = ft_substr(str, start, *i - start);
	if (quote == '\"')
	{
		expanded = expand_variables(shell, temp, 1);
		free(temp);
		return (expanded);
	}
	return (temp);
}

static char	*expand_segment_wildcards(char *str)
{
	char	**matches;
	char	*result;

	if (!has_wildcards(str))
		return (ft_strdup(str));
	matches = expand_wildcards(str);
	if (!matches)
		return (ft_strdup(str));
	result = join_expanded_wildcards(matches);
	free_array(matches);
	return (result);
}

char	*process_unquoted_segment(t_shell *shell, char *str, int *i)
{
	int		start;
	char	*temp;
	char	*expanded;
	char	*wildcarded;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '\"')
		(*i)++;
	temp = ft_substr(str, start, *i - start);
	expanded = expand_variables(shell, temp, 0);
	free(temp);
	wildcarded = expand_segment_wildcards(expanded);
	free(expanded);
	return (wildcarded);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

char	*init_result(void)
{
	char	*result;

	result = ft_strdup("");
	return (result);
}

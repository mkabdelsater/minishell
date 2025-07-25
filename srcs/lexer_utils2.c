/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamaha <csamaha@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:10:51 by csamaha           #+#    #+#             */
/*   Updated: 2025/07/09 16:10:52 by csamaha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type, int quoted)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->quoted = quoted;
	token->next = NULL;
	return (token);
}

void	add_to_token_list(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

int	handle_parentheses(t_shell *shell, char *input, int *i)
{
	if (input[*i] == '(')
	{
		add_to_token_list(&shell->tokens, create_token("(", TOKEN_LPAREN, 0));
		(*i)++;
		return (1);
	}
	else if (input[*i] == ')')
	{
		add_to_token_list(&shell->tokens, create_token(")", TOKEN_RPAREN, 0));
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_pipe_or(t_shell *shell, char *input, int *i)
{
	if (input[*i] == '|')
	{
		if (input[*i + 1] == '|')
		{
			add_to_token_list(&shell->tokens, create_token("||", TOKEN_OR, 0));
			(*i) += 2;
		}
		else
		{
			add_to_token_list(&shell->tokens, create_token("|", TOKEN_PIPE, 0));
			(*i)++;
		}
		return (1);
	}
	return (0);
}

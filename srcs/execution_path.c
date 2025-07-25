/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamaha <csamaha@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:59:56 by csamaha           #+#    #+#             */
/*   Updated: 2025/07/09 15:59:57 by csamaha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*check_direct_path(char *cmd)
{
	if (!cmd || ft_strlen(cmd) == 0)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

char	*search_in_path(char *cmd, char *path_env)
{
	char	**paths;
	char	*path;
	int		i;

	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		path = ft_strjoin_free(path, cmd);
		if (access(path, X_OK) == 0)
		{
			free_array(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

char	*find_command_path(t_shell *shell, char *cmd)
{
	char	*direct_path;
	char	*path_env;
	char	*expanded_cmd;

	expanded_cmd = expand_token(shell, cmd);
	if (!expanded_cmd)
		return (NULL);
	direct_path = check_direct_path(expanded_cmd);
	if (direct_path)
	{
		free(expanded_cmd);
		return (direct_path);
	}
	path_env = get_env_value(shell, "PATH");
	direct_path = search_in_path(expanded_cmd, path_env);
	free(expanded_cmd);
	return (direct_path);
}

int	execute_single_command(t_shell *shell, t_command *cmd)
{
	int	result;
	int	stdin_backup;
	int	stdout_backup;

	if (!cmd->args)
		return (0);
	if (is_builtin(cmd->args[0]))
	{
		if (setup_builtin_redirects(shell, cmd, &stdin_backup, &stdout_backup))
			return (1);
		result = execute_builtin(shell, cmd);
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (result);
	}
	else
		return (execute_external_command(shell, cmd));
}

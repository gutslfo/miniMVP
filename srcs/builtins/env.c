/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:37:39 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 19:44:08 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	print_variable(t_ast *node, const char *name)
{
	char	*var;
	char	*str;
	int		str_len;
	int		len;

	str = get_str(name);
	if (str)
	{
		str_len = ft_strlen(str);
		len = str_len + ft_strlen(name) + 2;	// + 2 accounts for the added '=' and the NULL-termination of the string
		var = (char *) malloc(len * sizeof(char));
		if (!var)
			malloc_error(node);
		ft_strlcat(var, name, len);
		ft_strlcat(var, "=", len);
		ft_strlcat(var, str, len);
		ft_putstr_fd(var, node->cmd.fd_out);
		ft_putchar_fd('\n', node->cmd.fd_out);
		free(var);
	}
}

int	count_variables(t_ast *node)
{
	int	count;

	if (!node->root->envp)
		return (0);
	count = 0;
	while (node->root->envp[count])
		count++;
	return (count);
}

char	*get_variable(t_ast *env, char *full)
{
	char	*var;
	int		i;

	i = 0;
	while (full[i] != '=')
		i++;
	var = (char *) malloc(i * sizeof(char));
	if (!var)
		malloc_error(env);
	i = -1;
	while (full[++i] != '=')
		var[i] = full[i];
	var[i] = 0;
	return (var);
}

char	**get_env_variables(t_ast *node, int count)
{
	char	**variables;
	int		i;

	variables = (char **) malloc(count * sizeof(char *));
	if (!variables)
		malloc_error(node);
	i = -1;
	while (node->root->envp[++i])
		variables[i] = get_variable(node, node->root->envp[i]);
	return (variables);
}

int	env(t_ast *node)
{
	char	**variables;
	int		count;
	int		i;

	if (make_redirs(node) == FAILURE)
		return (FAILURE);
	count = count_variables(node);
	if (count)
		variables = get_env_variables(node, count);
	i = -1;
	while (++i < count)
	{
		print_variable(node, variables[i]);
		free(variables[i]);
	}
	free(variables);
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (SUCCESS);
}

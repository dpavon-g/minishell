/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 00:23:56 by ldurante          #+#    #+#             */
/*   Updated: 2021/12/10 16:34:58 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* REVISAR FREES, NORMA, IMPLEMENTAR "$?" */

void	insert_var(t_input *in, char *var, char *first, int j)
{
	int		len1;
	int		len2;
	int		len3;
	char	*last;
	char	*expanded;

	expanded = ft_strdup("");
	len1 = ft_strlen(first);
	len2 = ft_strlen(var);
	len3 = ft_strlen(in->split_input[j]);
	last = ft_substr(in->split_input[j], len1 + len2 + 1, len3 - len2 - len1);
	if (!strncmp(var, "$", 2))
		expanded = ft_strdup(var);
	else
		expanded = ft_getenv(var, in);
	free(var);
	if (!expanded)
		expanded = ft_strdup("");
	in->split_input[j] = ft_strjoin3(first, expanded, last);
	free(first);
	free(expanded);
	free(last);
}

void	replace_var(t_input *in, int front, char *first, char *var)
{
	if (!(ft_isalnum(in->split_input[in->flags.j][in->flags.i + 1])))
	{
		if (in->split_input[in->flags.j][in->flags.i + 1] == '\0')
			var = ft_strdup("$");
		else if (in->split_input[in->flags.j][in->flags.i + 1] == '"')
		{
			while (in->split_input[in->flags.j][in->flags.i] == '"')
				in->flags.i++;
			if (in->flags.i % 2 != 0)
				var = ft_strdup("$");
		}
		else if (in->split_input[in->flags.j][in->flags.i + 1] == '?')
			printf("LAST ERROR\n");
		else
			write(1, "$", 1);
	}
	else
	{
		in->flags.i++;
		while (ft_isalnum(in->split_input[in->flags.j][in->flags.i]))
			in->flags.i++;
		var = ft_substr(in->split_input[in->flags.j],
				front + 1, in->flags.i - 1 - front);
	}
	first = ft_substr(in->split_input[in->flags.j], 0, front);
	insert_var(in, var, first, in->flags.j);
}

void	expand_vars(t_input *in)
{
	int		front;
	char	*var;
	char	*first;

	var = ft_strdup("");
	first = ft_strdup("");
	ft_bzero(&in->flags, sizeof(in->flags));
	while (in->split_input[in->flags.j] != NULL)
	{
		in->flags.i = 0;
		in->flags.single_q = 0;
		in->flags.double_q = 0;
		front = 0;
		while (in->split_input[in->flags.j][in->flags.i] != '\0')
		{
			front = in->flags.i;
			quotes_aux(in, in->split_input[in->flags.j]);
			if (in->split_input[in->flags.j][in->flags.i] == '$'
				&& in->flags.single_q != 1)
				replace_var(in, front, first, var);
			in->flags.i++;
		}
		in->flags.j++;
	}
	in->split_input = quotes(in);
}
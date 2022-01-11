/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 12:55:39 by ldurante          #+#    #+#             */
/*   Updated: 2022/01/11 20:00:54 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pair_quotes(t_input *in)
{
	ft_bzero(&in->flags, sizeof(in->flags));
	while (in->user_input[in->flags.i])
	{
		if (in->user_input[in->flags.i] == '"' && !in->flags.single_q
			&& !in->flags.double_q)
		{
			in->flags.count_double++;
			in->flags.double_q = 1;
		}
		else if (in->user_input[in->flags.i] == '"' && !in->flags.single_q
			&& in->flags.double_q && (in->flags.count_double++))
			in->flags.double_q = 0;
		else if (in->user_input[in->flags.i] == '\'' && !in->flags.single_q
			&& !in->flags.double_q)
		{
			in->flags.count++;
			in->flags.single_q = 1;
		}
		else if (in->user_input[in->flags.i] == '\'' && in->flags.single_q
			&& !in->flags.double_q && (in->flags.count++))
			in->flags.single_q = 0;
		in->flags.i++;
	}
	return (in->flags.count_double % 2 + in->flags.count % 2);
}

void	check_quotes(t_input *in)
{
	if (in->user_input[in->flags.i] == '"' && !in->flags.single_q
		&& !in->flags.double_q)
		in->flags.double_q = 1;
	else if (in->user_input[in->flags.i] == '"' && !in->flags.single_q
		&& in->flags.double_q)
		in->flags.double_q = 0;
	else if (in->user_input[in->flags.i] == '\'' && !in->flags.single_q
		&& !in->flags.double_q)
		in->flags.single_q = 1;
	else if (in->user_input[in->flags.i] == '\'' && in->flags.single_q
		&& !in->flags.double_q)
		in->flags.single_q = 0;
}

int	check_errors2(t_input *in)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (in->user_input[i])
	{
		if (in->user_input[i] != ' ' && in->user_input[i] != '|')
			flag = 1;
		if (in->user_input[i] == '|' && flag == 0)
		{
			error_msg(in, ERR_SYNTAX, -1);
			return (1);
		}
		i++;
	}
	return(0);
}

int	check_errors(t_input *in)
{
	char	c;
	int		count;
	int		flag_diff;
	int		special;

	special = 0;
	flag_diff = 0;
	while (in->user_input[in->flags.i])
	{
		count = 0;
		check_quotes(in);
		if (in->flags.double_q == 0 && in->flags.single_q == 0)
		{
			if (in->user_input[in->flags.i] == '<' || in->user_input[in->flags.i] == '>'
				|| in->user_input[in->flags.i] == '|' || in->user_input[in->flags.i] == ' ')
			{
				if (in->user_input[in->flags.i] == '<' || in->user_input[in->flags.i] == '>'
					|| in->user_input[in->flags.i] == '|')
				{
					if (in->user_input[in->flags.i] == '|' && special == 1 && flag_diff == 0)
					{
						error_msg(in, ERR_SYNTAX, -2);
						return (1);
					}
					else if (in->user_input[in->flags.i] == '|')
					{
						count = 0;
						flag_diff = 0;
					}
					special = 1;
				}
				c = in->user_input[in->flags.i];
				while (c == in->user_input[in->flags.i] && count <= 2)
				{
					count++;
					in->flags.i++;
				}
				while (in->user_input[in->flags.i] == ' ')
					in->flags.i++;
				if (((c == '<' || c == '>') && count > 2) || (c == '|' && count > 1)
					|| in->user_input[in->flags.i] == c)
				{
					error_msg(in, ERR_SYNTAX, -2);
					return (1);
				}
			}
			else
			{
				flag_diff = 1;
				in->flags.i++;
			}
		}
		else
			in->flags.i++;
	}
	if (flag_diff == 0 && special == 1)
	{
		error_msg(in, ERR_SYNTAX, -2);
		return (1);
	}
	return (check_errors2(in));
}

void	read_input_aux(t_input *in, char *aux)
{
	if (in->user_input[0] != '\0')
		add_history(in->user_input);
	ft_bzero(&in->flags, sizeof(in->flags));
	if (!check_errors(in))
	{
		aux = in->user_input;
		in->user_input = split_pipes(in);
		free(aux);
		if (check_args(in))
		{
			if (is_builtin(in) && count_pipes(in) == 1)
			{
				check_redirs(in);
				if (!in->is_err)
					exec_args(in);
				if (in->is_outfile)
				{
					dup2(in->back_stdout, STDOUT_FILENO);
					close(in->back_stdout);
				}
				if (!in->is_err)
					exit_status = 0;
			}
			else
				init_arg_list(in);
		}
	}
}

void	read_input(t_input *in)
{
	char	*user;
	char	*aux;
	char	*color;

	aux = NULL;
	user = ft_getenv("USER", in);
	if (!user)
		user = ft_strdup("guest");
	if (exit_status != 0)
		color = ft_strdup("\033[0;31m$ \033[0m");
	else
		color = ft_strdup("\033[0;32m$ \033[0m");
	// in->prompt = ft_strjoin(user, "@minishell> $ ");
	in->prompt = ft_strjoin3(user, "@minishell> ", color);
	free(color);
	in->user_input = readline(in->prompt);
	in->is_err = 0;
	in->quote_state = malloc(1);
	if (in->user_input)
	{
		if ((ft_strncmp(in->user_input, "", 1)))
		{
			if (pair_quotes(in) == 0)
				read_input_aux(in, aux);
			else
			{
				error_msg(in, ERR_ARG, -2);
				add_history(in->user_input);
			}
		}
		if (in->split_input)
		{	
			free_matrix(in->split_input);
			in->split_input = NULL;
		}
		free(in->quote_state);
		free(in->user_input);
		free(in->prompt);
		free(user);
	}
	else
	{
		free(in->quote_state);
		write(1, "exit\n", 5);
		ft_lstclear(in->env_list, free);
		free(in->prompt);
		free(user);
		exit(0);
	}
}



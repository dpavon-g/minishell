/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 19:23:22 by ldurante          #+#    #+#             */
/*   Updated: 2021/11/16 21:29:19 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	double_quotes(t_input *in, const char *s)
{
	while (s[in->flags.i] != '"' && s[in->flags.i] != '\0')
		in->flags.i++;
	in->flags.double_q = 0;
	if (!in->flags.double_q && s[in->flags.i+1] != ' ')
	{
		while (s[in->flags.i] && s[in->flags.i] != ' ')
			in->flags.i++;
		in->flags.count++;
		in->flags.global_count = 1;
	}
	else
	{
		in->flags.count++;
		in->flags.global_count = 1;
	}
}

void	single_quotes(t_input *in, const char *s)
{
	while (s[in->flags.i] != '\'' && s[in->flags.i] != '\0')
		in->flags.i++;
	in->flags.single_q = 0;
	if (!in->flags.single_q && s[in->flags.i+1] != ' ')
	{
		while (s[in->flags.i] && s[in->flags.i] != ' ')
			in->flags.i++;
		in->flags.count++;
		in->flags.global_count = 1;
	}
	else
	{
		in->flags.count++;
		in->flags.global_count = 1;
	}
}

// void	check_quotes(t_input *in, const char *s)
// {
// 	if (s[in->flags.i] == '\'' && in->flags.double_q == 0)
// 	{
// 		in->flags.single_q = 1;
// 		in->flags.global_q = 1;
// 	}
// 	else if (s[in->flags.i] == '"' && in->flags.single_q == 0)
// 	{
// 		in->flags.double_q = 1;
// 		in->flags.global_q = 1;
// 	}
// 	else if (in->flags.double_q || in->flags.single_q)
// 	{
// 		if (in->flags.double_q)
// 			double_quotes(in, s);
// 		if (in->flags.single_q)
// 			single_quotes(in, s);
// 		in->flags.global_sp = 1;
// 	}
// }

void	split_args(t_input *in, const char *s)
{
	if (!in->flags.global_q)
	{
		if (s[in->flags.i - 1] != ' ')
			in->split_input[in->flags.j] = ft_substr(s,	in->flags.start,
				in->flags.i - in->flags.start);
		else	
			in->split_input[in->flags.j] = ft_substr(s,	in->flags.start,
				in->flags.i - in->flags.start - 1);
	}
	else if (in->flags.global_sp)
	{
		in->split_input[in->flags.j] = ft_substr(s,	in->flags.start,
				in->flags.i - in->flags.start);
		in->flags.global_sp = 0;
		in->flags.global_q = 0;
	}	
	if (in->split_input[in->flags.j])
		in->flags.j++;
	in->flags.global_count = 0;
}

int		count_tokens(const char *s, t_input *in, int split)
{
	init_flags(in);
	while (s[in->flags.i] != '\0')
	{
		if (s[in->flags.i] == '\'' && in->flags.double_q == 0)
		{
			in->flags.single_q = 1;
			in->flags.global_q = 1;
		}
		else if (s[in->flags.i] == '"' && in->flags.single_q == 0)
		{
			in->flags.double_q = 1;
			in->flags.global_q = 1;
		}
		else if (in->flags.double_q || in->flags.single_q)
		{
			if (in->flags.double_q)
				double_quotes(in, s);
			if (in->flags.single_q)
				single_quotes(in, s);
			in->flags.global_sp = 1;
		}
		else
		{
			if (s[in->flags.i] != ' ' && s[in->flags.i] != '\0')
			{
				in->flags.count++;
				while (s[in->flags.i] != ' ' && s[in->flags.i])
					in->flags.i++;
				in->flags.global_count = 1;
			}
		}
		if (s[in->flags.i])
			in->flags.i++;
		if (split == 1 && in->flags.global_count)
			split_args(in, s);
		if (!in->flags.global_q)
			in->flags.start = in->flags.i;
	}
	return (in->flags.count);
}

char	**check_args(t_input *in)
{
	int		input_size;

	if (!in->user_input)
		return (NULL);
	input_size = count_tokens(in->user_input, in, 0);
	// printf("Number: %d\n",	input_size);
	in->split_input = malloc(sizeof(char *) * (input_size + 1));
	if (!in->split_input)
		return (NULL);
	count_tokens(in->user_input, in, 1);
	in->split_input[input_size] = NULL;
	// print_matrix(in->split_input);
	return (in->split_input);
}

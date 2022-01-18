/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 12:33:39 by ldurante          #+#    #+#             */
/*   Updated: 2022/01/18 13:42:06 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cd(t_input *in)
{
	char	*home_path;
	char	*pwd;

	home_path = ft_getenv("HOME", in);
	pwd = getcwd(NULL, 0);
	if (!in->split_in[1] || !(ft_strncmp(in->split_in[1], "", 2)))
	{
		if (chdir(home_path) == -1)
			error_msg(in, ERR_HOME, 0, 0);
		else
			update_env_var(in, "OLDPWD=", pwd);
	}
	else if (chdir(in->split_in[1]) != 0)
	{
		if (errno == EACCES)
			error_msg(in, ERR_PERM, 0, 0);
		else
			error_msg(in, ERR_FILE, 1, 0);
		if (!pwd)
		{
			update_env_var(in, "OLDPWD=", "");
			chdir("/");
			pwd = getcwd(NULL, 0);
		}
	}
	else
	{
		if (pwd)
			update_env_var(in, "OLDPWD=", pwd);
		else
			update_env_var(in, "OLDPWD=", "");
	}
	free(pwd);
	pwd = getcwd(NULL, 0);
	update_env_var(in, "PWD=", pwd);
	free(home_path);
	free(pwd);
}

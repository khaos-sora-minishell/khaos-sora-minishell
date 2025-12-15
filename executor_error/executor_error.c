/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:42:42 by akivam            #+#    #+#             */
/*   Updated: 2025/12/14 21:49:26 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* src/executor_error/executor_error.c */

#include "minishell.h" // Libft ve structlar için
#include "executor_error.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

/*
** Genel hata yazdırıcı
** Çıktı: minishell: [cmd]: [msg]
*/
void	print_execution_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(msg, 2);
}

/*
** Durum: Komut PATH içinde bulunamadı.
** Eylem: Hata basar ve 127 ile çıkar.
*/
void	handle_cmd_not_found(char *cmd)
{
	print_execution_error(cmd, ERR_CMD_NOT_FOUND);
	exit(EXIT_CMD_NOT_FOUND);
}

/*
** Durum: Dosya var ama çalıştırma yetkisi yok (EACCES).
** Eylem: Hata basar ve 126 ile çıkar.
*/
void	handle_permission_denied(char *cmd)
{
	print_execution_error(cmd, ERR_PERM_DENIED);
	exit(EXIT_CANNOT_EXEC);
}

/*
** Durum: Verilen yol bir klasör (EISDIR).
** Eylem: Hata basar ve 126 ile çıkar.
*/
void	handle_is_directory(char *cmd)
{
	print_execution_error(cmd, ERR_IS_DIR);
	exit(EXIT_CANNOT_EXEC);
}

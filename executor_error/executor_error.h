/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:42:24 by akivam            #+#    #+#             */
/*   Updated: 2025/12/14 21:49:26 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* src/executor_error/executor_error.h */

#ifndef EXECUTOR_ERROR_H
# define EXECUTOR_ERROR_H

/* --- Bash Standart Hata Mesajları --- */
# define ERR_CMD_NOT_FOUND "command not found"
# define ERR_NO_FILE "No such file or directory"
# define ERR_PERM_DENIED "Permission denied"
# define ERR_IS_DIR "Is a directory"

/* --- Bash Çıkış Kodları (Exit Status) --- */
/* execve başarısız olduğunda dönecek değerler */
# define EXIT_CMD_NOT_FOUND 127 // PATH'te yoksa
# define EXIT_CANNOT_EXEC 126   // Dosya var ama çalışmıyor (Yetki/Klasör)
# define EXIT_GENERAL 1         // Diğer genel hatalar

/* --- Fonksiyon Prototipleri --- */
void    print_execution_error(char *cmd, char *msg);
void    handle_cmd_not_found(char *cmd);
void    handle_permission_denied(char *cmd);
void    handle_is_directory(char *cmd);

#endif
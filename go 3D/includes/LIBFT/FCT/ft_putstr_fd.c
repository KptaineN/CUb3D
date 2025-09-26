/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:44:31 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:45 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		return ;
	while (*str)
	{
		ft_putchar_fd(*str, fd);
		str++;
	}
}
/*#include "libft.h"
#include <fcntl.h> // Pour open
#include <unistd.h> // Pour close

int main(void)
{
    int fd;

    // Ouvrir un fichier pour écrire
    fd = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    // Utiliser ft_putstr_fd pour écrire dans le fichier
    ft_putstr_fd("Hello, world!", fd);

    // Fermer le fichier
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }

    // Afficher un message de succès
    printf("String written to test_output.txt\n");

    return 0;
}*/

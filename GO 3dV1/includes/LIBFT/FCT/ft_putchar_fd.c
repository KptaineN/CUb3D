/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:41:01 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:38 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
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

    // Utiliser ft_putchar_fd pour écrire dans le fichier
    ft_putchar_fd('H', fd);
    ft_putchar_fd('e', fd);
    ft_putchar_fd('l', fd);
    ft_putchar_fd('l', fd);
    ft_putchar_fd('o', fd);
    ft_putchar_fd('\n', fd);

    // Fermer le fichier
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }

    // Afficher un message de succès
    printf("Characters written to test_output.txt\n");

    return 0;
}*/
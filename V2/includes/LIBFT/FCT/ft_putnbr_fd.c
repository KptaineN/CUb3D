/* ************************************************************************** */
/* 									      */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:00:15 by nkiefer           #+#    #+#             */
/*   Updated: 2024/10/30 06:31:55 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n = -n;
	}
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	else
		ft_putchar_fd(n + '0', fd);
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

    // Utiliser ft_putnbr_fd pour écrire dans le fichier
    ft_putnbr_fd(12345, fd);
    ft_putchar_fd('\n', fd);
    ft_putnbr_fd(-12345, fd);
    ft_putchar_fd('\n', fd);
    ft_putnbr_fd(0, fd);
    ft_putchar_fd('\n', fd);
    ft_putnbr_fd(2147483647, fd);
    ft_putchar_fd('\n', fd);
    ft_putnbr_fd(-2147483648, fd);
    ft_putchar_fd('\n', fd);

    // Fermer le fichier
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }

    // Afficher un message de succès
    printf("Numbers written to test_output.txt\n");

    return 0;
}*/

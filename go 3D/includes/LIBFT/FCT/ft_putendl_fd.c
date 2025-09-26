/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:50:16 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:00:41 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_putendl_fd(char *str, int fd)
{
	if (!str)
		return ;
	while (*str)
	{
		ft_putchar_fd(*str, fd);
		str++;
	}
	ft_putchar_fd('\n', fd);
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

	// Utiliser ft_putendl_fd pour écrire dans le fichier
	ft_putendl_fd("Hello, world!", fd);

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

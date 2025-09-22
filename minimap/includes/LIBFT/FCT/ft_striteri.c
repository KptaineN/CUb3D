/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiefer <nkiefer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:27:14 by nkiefer           #+#    #+#             */
/*   Updated: 2025/08/13 14:01:37 by nkiefer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (!s || !f)
		return ;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*#include "libft.h"
#include <stdio.h>

void example_function(unsigned int i, char *c)
{
    if (i % 2 == 0)
    {
        *c = ft_toupper(*c);
    }
    else
    {
        *c = ft_tolower(*c);
    }
}

int main(void)
{
    char str[] = "Hello, World!";

    printf("Original string: %s\n", str);

    ft_striteri(str, example_function);

    printf("Modified string: %s\n", str);

    return 0;
}*/
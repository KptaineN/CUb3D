/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 11:24:57 by nkiefer           #+#    #+#             */
/*   Updated: 2025/09/21 18:24:50 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include "GNL/get_next_line.h"
# include "PRINTF/ft_printf.h"
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

long long	ft_atoll(const char *nptr);
long	    ft_atol(const char *str);
int		    ft_isspace(int c);
int		    ft_atoi(const char *nptr);
int		    ft_isalnum(int c);
int		    ft_isalpha(int c);
int		    ft_isascii(int c);
int		    ft_isdigit(int c);
int		    ft_isprint(int c);
int		    ft_tolower(int c);
int		    ft_toupper(int c);
int		    ft_strncmp(const char *s1, const char *s2, size_t n);
int		    ft_memcmp(const void *s1, const void *s2, size_t n);
int		    ft_strcmp(const char *s1, const char *s2);
int		    ft_printf(const char *format, ...);
int		    ft_putchar(char c);
void	    ft_swap(int *a, int *b);
void	    ft_swap_str(char **a, char **b);
void	    *ft_memchr(const void *str, int c, size_t n);
void	    *ft_memcpy(void *dst, const char *src, size_t n);
void	    *ft_memmove(void *dest, const void *src, size_t len);
void	    *ft_memset(void *s, int c, size_t len);
void	    *ft_calloc(size_t elal, size_t size);
void	    ft_bzero(void *s, size_t n);
void	    ft_striteri(char *s, void (*f)(unsigned int, char *));
void	    ft_putchar_fd(char c, int fd);
void	    ft_putstr_fd(char *str, int fd);
void	    ft_putendl_fd(char *s, int fd);
void	    ft_putnbr_fd(int n, int fd);
void	    ft_bubble_str_sort(char **tab);
char	    *get_next_line(int fd);
char	    *ft_strchr(const char *str, int c);
char	    *ft_strdup(const char *s);
char	    *ft_strjoin(char const *s1, char const *s2);
char	    *ft_substr(char const *s, unsigned int start, size_t len);
char	    *ft_strtrim(char const *s1, char const *set);
char	    *ft_strnstr(const char *big, const char *little, size_t len);
char	    *ft_strstr(const char *big, const char *little);
char	    *ft_strrchr(const char *str, int c);
char	    **ft_split(char const *s, char c);
char	    *ft_itoa(int n);
char	    *ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	    *ft_strcpy(char *dest, const char *src);
char	    *ft_strndup(const char *s, size_t n);
char	    *ft_strncpy(char *dest, const char *src, size_t n);
const char  *ft_skip_spaces(const char *str);
size_t	    ft_strlen(const char *str);
size_t	    ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	    ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	    ft_strcspn(const char *s, const char *reject);

#endif /* LIBFT_H */

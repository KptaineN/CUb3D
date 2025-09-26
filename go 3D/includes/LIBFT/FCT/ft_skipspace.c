#include "../libft.h"

const char    *ft_skip_spaces(const char *str)
{
    if (!str)
        return (NULL);
    while (ft_isspace(*str))
        str++;
    return (str);
}

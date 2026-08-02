/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:19:56 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/30 21:43:27 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;
	char	ch;

	ptr = (char *)s + ft_strlen(s);
	ch = (char)c;
	while (ptr >= s)
	{
		if (*ptr == ch)
		{
			return (ptr);
		}
		ptr--;
	}
	return (NULL);
}

/* int main(void)
{
    const char *str = "Hello, world!";
    char ch = 'o';
    char *result;

    // Test case 1: Character is in the string
    result = ft_strrchr(str, ch);
    if (result)
        printf("Character '%c' found at position: %ld\n", ch, result - str);
    else
        printf("Character '%c' not found\n", ch);

    // Test case 2: Character is not in the string
    ch = 'z';
    result = ft_strrchr(str, ch);
    if (result)
        printf("Character '%c' found at position: %ld\n", ch, result - str);
    else
        printf("Character '%c' not found\n", ch);

    // Test case 3: Null character
    ch = '\0';
    result = ft_strrchr(str, ch);
    if (result)
        printf("Null character found at position: %ld\n", result - str);
    else
        printf("Null character not found\n");

    return (0);
} */